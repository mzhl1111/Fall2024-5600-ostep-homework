#include <linux/userfaultfd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static volatile int stop;

struct params {
    int uffd;
    long page_size;
};

static inline uint64_t getns(void)
{
    struct timespec ts;
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    assert(ret == 0);
    return (((uint64_t)ts.tv_sec) * 1000000000ULL) + ts.tv_nsec;
}

static long get_page_size(void)
{
    long ret = sysconf(_SC_PAGESIZE);
    if (ret == -1) {
        perror("sysconf/pagesize");
        exit(1);
    }
    assert(ret > 0);
    return ret;
}

static void *handler(void *arg)
{
    struct params *p = arg;
    long page_size = p->page_size;
    char buf[page_size];
    memset(buf, 'a', sizeof(buf));

    for (;;) {
        struct uffd_msg msg;

        struct pollfd pollfd[1];
        pollfd[0].fd = p->uffd;
        pollfd[0].events = POLLIN;

        // wait for a userfaultfd event to occur
        int pollres = poll(pollfd, 1, 2000);

        if (stop)
            return NULL;

        switch (pollres) {
        case -1:
            perror("poll/userfaultfd");
            continue;
        case 0:
            continue;
        case 1:
            break;
        default:
            fprintf(stderr, "unexpected poll result\n");
            exit(1);
        }

        if (pollfd[0].revents & POLLERR) {
            fprintf(stderr, "pollerr\n");
            exit(1);
        }

        if (!pollfd[0].revents & POLLIN) {
            continue;
        }

        int readres = read(p->uffd, &msg, sizeof(msg));
        if (readres == -1) {
            if (errno == EAGAIN)
                continue;
            perror("read/userfaultfd");
            exit(1);
        }

        if (readres != sizeof(msg)) {
            fprintf(stderr, "invalid msg size\n");
            exit(1);
        }

        // handle the page fault by copying a page worth of bytes
        if (msg.event & UFFD_EVENT_PAGEFAULT) {
            long long addr = msg.arg.pagefault.address;
            struct uffdio_copy copy;
            copy.src = (long long)buf;
            copy.dst = (long long)addr;
            copy.len = page_size;
            copy.mode = 0;
            if (ioctl(p->uffd, UFFDIO_COPY, &copy) == -1) {
                perror("ioctl/copy");
                exit(1);
            }
        }

    }

    return NULL;
}

int main(int argc, char **argv)
{
    int uffd;
    long page_size;
    long num_pages;
    void *region;
    pthread_t uffd_thread;

    page_size = get_page_size();
    num_pages = 20;

    // open the userfault fd
    uffd = syscall(__NR_userfaultfd, O_CLOEXEC | O_NONBLOCK);
    if (uffd == -1) {
        perror("syscall/userfaultfd");
        exit(1);
    }

    // enable for api version and check features
    struct uffdio_api uffdio_api;
    uffdio_api.api = UFFD_API;
    uffdio_api.features = 0;
    if (ioctl(uffd, UFFDIO_API, &uffdio_api) == -1) {
        perror("ioctl/uffdio_api");
        exit(1);
    }

    if (uffdio_api.api != UFFD_API) {
        fprintf(stderr, "unsupported userfaultfd api\n");
        exit(1);
    }

    // allocate a memory region to be managed by userfaultfd
    region = mmap(NULL, page_size * num_pages, PROT_READ|PROT_WRITE,
            MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // register the pages in the region for missing callbacks
    struct uffdio_register uffdio_register;
    uffdio_register.range.start = (unsigned long)region;
    uffdio_register.range.len = page_size * num_pages;
    uffdio_register.mode = UFFDIO_REGISTER_MODE_MISSING;
    if (ioctl(uffd, UFFDIO_REGISTER, &uffdio_register) == -1) {
        perror("ioctl/uffdio_register");
        exit(1);
    }

    if ((uffdio_register.ioctls & UFFD_API_RANGE_IOCTLS) !=
            UFFD_API_RANGE_IOCTLS) {
        printf("Supported ioctls: 0x%llx\n", (unsigned long long)uffdio_register.ioctls);
        printf("Expected ioctls: 0x%llx\n", (unsigned long long)UFFD_API_RANGE_IOCTLS);

        fprintf(stderr, "unexpected userfaultfd ioctl set\n");
        exit(1);
    }


    // start the thread that will handle userfaultfd events
    stop = 0;

    struct params p;
    p.uffd = uffd;
    p.page_size = page_size;

    pthread_create(&uffd_thread, NULL, handler, &p);

    sleep(1);

    // track the latencies for each page
    uint64_t *latencies = malloc(sizeof(uint64_t) * num_pages);
    assert(latencies);
    memset(latencies, 0, sizeof(uint64_t) * num_pages);

    // touch each page in the region
    int value;
    char *cur = region;
    fprintf(stdout, "expected value: %d\n", (int)'a');
    for (long i = 0; i < num_pages; i++) {
        uint64_t start = getns();
        char v = *((char*)cur);
        uint64_t dur = getns() - start;
        latencies[i] = dur;
        v = *((char*)cur);
        fprintf(stdout, "Modified page value: %d\n", v);
        value += v;
        cur += page_size;
    }

    stop = 1;
    pthread_join(uffd_thread, NULL);

    if (ioctl(uffd, UFFDIO_UNREGISTER, &uffdio_register.range)) {
        fprintf(stderr, "ioctl unregister failure\n");
        return 1;
    }

    long long sum = 0;
    fprintf(stdout, "Frist mapping latency: %lld\n", (unsigned long long)latencies[0]);
    for (long i = 1; i < num_pages; i++) {
        sum += (unsigned long long)latencies[i];
    }
    double average = (double)sum / num_pages;
    fprintf(stdout, "Rest average latency: %.2f\n", average);

    free(latencies);
    munmap(region, page_size * num_pages);

    return 0;
}