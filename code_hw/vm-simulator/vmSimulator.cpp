#include " vmSimulator.h"

// VirtualAddress Constructor
VirtualAddress::VirtualAddress(int address, int pageSize) {
    pageNumber = address / pageSize;
    offset = address % pageSize;
}

// PhysicalAddress Constructor
PhysicalAddress::PhysicalAddress(int frameNum, int offset) : frameNumber(frameNum), offset(offset) {}

Frame::Frame(int num) : frameNumber(num), isFree(true) {}

PageTableEntry::PageTableEntry() : frameNumber(-1), flags(0) {}

TLBEntry::TLBEntry(int pageNum, int frameNum) : pageNumber(pageNum), frameNumber(frameNum) {}

PCB::PCB(int pid, int numPages) : processID(pid), pageTable(numPages) {
    codeSegment = { -1, 0 };
    stackSegment = { -1, 0 };
    heapSegment = { -1, 0 };
}

// VMSimulator Constructor
VMSimulator::VMSimulator(int pageSize, int virtualMemSize, int physicalMemSize, int tlbSize)
    : PAGE_SIZE(pageSize),
      VIRTUAL_MEMORY_SIZE(virtualMemSize),
      PHYSICAL_MEMORY_SIZE(physicalMemSize),
      TLB_SIZE(tlbSize),
      nextProcessID(1)
{
    NUM_PAGES = VIRTUAL_MEMORY_SIZE / PAGE_SIZE;
    NUM_FRAMES = PHYSICAL_MEMORY_SIZE / PAGE_SIZE;

    initializePhysicalMemory();
}

// Initializes physical memory frames
void VMSimulator::initializePhysicalMemory() {
    for (int i = 0; i < NUM_FRAMES; ++i) {
        physicalMemory.emplace_back(i);
    }
}

// Bit manipulation helper functions
void VMSimulator::setFlag(unsigned int &flags, unsigned int mask) {
    flags |= mask;
}

void VMSimulator::clearFlag(unsigned int &flags, unsigned int mask) {
    flags &= ~mask;
}

bool VMSimulator::checkFlag(unsigned int flags, unsigned int mask) {
    return (flags & mask) != 0;
}

// Creates a new process and returns its process ID
int VMSimulator::createProcess() {
    int pid = nextProcessID++;
    processes.emplace(pid, PCB(pid, NUM_PAGES));
    std::cout << "Created process with PID: " << pid << std::endl;
    return pid;
}

// Allocates pages with given protection flags; returns starting page number or -1 on failure
int VMSimulator::allocatePages(PCB &pcb, int numPagesNeeded, unsigned int protectionFlags) {
    // Best-fit algorithm variables
    int bestStartFrame = -1;
    int bestSize = NUM_FRAMES + 1;

    int currentStart = -1;
    int currentSize = 0;

    // Find best-fit block in physical memory
    for (int i = 0; i < NUM_FRAMES; ++i) {
        if (physicalMemory[i].isFree) {
            if (currentStart == -1) currentStart = i;
            currentSize++;
        } else {
            if (currentSize >= numPagesNeeded && currentSize < bestSize) {
                bestStartFrame = currentStart;
                bestSize = currentSize;
            }
            currentStart = -1;
            currentSize = 0;
        }
    }

    // Check at the end
    if (currentSize >= numPagesNeeded && currentSize < bestSize) {
        bestStartFrame = currentStart;
        bestSize = currentSize;
    }

    if (bestStartFrame == -1) {
        std::cerr << "Not enough physical memory to allocate." << std::endl;
        return -1;
    }

    // Allocate pages and map to frames
    int startPage = -1;
    int pagesAllocated = 0;
    for (int i = 0; i < NUM_PAGES && pagesAllocated < numPagesNeeded; ++i) {
        if (!checkFlag(pcb.pageTable[i].flags, VALID_BIT)) {
            if (startPage == -1) startPage = i;
            pcb.pageTable[i].frameNumber = bestStartFrame + pagesAllocated;
            setFlag(pcb.pageTable[i].flags, VALID_BIT | protectionFlags);
            physicalMemory[bestStartFrame + pagesAllocated].isFree = false;
            pagesAllocated++;
        }
    }

    if (pagesAllocated < numPagesNeeded) {
        std::cerr << "Not enough virtual pages available." << std::endl;
        return -1;
    }

    return startPage;
}


void VMSimulator::allocateCodeSegment(PCB &pcb, int size) {
    int numPagesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int startPage = allocatePages(pcb, numPagesNeeded, READ_BIT);

    if (startPage != -1) {
        pcb.codeSegment = { startPage, numPagesNeeded };
        std::cout << "Allocated code segment for process " << pcb.processID << std::endl;
    } else {
        std::cerr << "Failed to allocate code segment for process " << pcb.processID << std::endl;
    }
}

// Allocates stack segment
void VMSimulator::allocateStackSegment(PCB &pcb, int size) {
    int numPagesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int startPage = allocatePages(pcb, numPagesNeeded, READ_BIT | WRITE_BIT);

    if (startPage != -1) {
        pcb.stackSegment = { startPage, numPagesNeeded };
        std::cout << "Allocated stack segment for process " << pcb.processID << std::endl;
    } else {
        std::cerr << "Failed to allocate stack segment for process " << pcb.processID << std::endl;
    }
}

// Allocates heap segment
void VMSimulator::allocateHeapSegment(PCB &pcb, int size) {
    int numPagesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int startPage = allocatePages(pcb, numPagesNeeded, READ_BIT | WRITE_BIT);

    if (startPage != -1) {
        pcb.heapSegment = { startPage, numPagesNeeded };
        std::cout << "Allocated heap segment for process " << pcb.processID << std::endl;
    } else {
        std::cerr << "Failed to allocate heap segment for process " << pcb.processID << std::endl;
    }
}

// Allocates additional heap memory
void VMSimulator::allocateHeapMemory(int processID, unsigned int size) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return;
    }
    PCB &pcb = it->second;

    int numPagesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int startPage = allocatePages(pcb, numPagesNeeded, READ_BIT | WRITE_BIT);

    if (startPage != -1) {
        if (pcb.heapSegment.startPage == -1) {
            pcb.heapSegment = { startPage, numPagesNeeded };
        } else {
            pcb.heapSegment.numPages += numPagesNeeded;
        }
        std::cout << "Allocated " << size << " bytes to heap for process " << processID << std::endl;
    } else {
        std::cerr << "Failed to allocate heap memory for process " << processID << std::endl;
    }
}

void MemorySimulator::freeHeapMemory(int processID, unsigned int virtualAddress) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return;
    }
    PCB &pcb = it->second;

    VirtualAddress vAddr(virtualAddress, PAGE_SIZE);


    Segment &heapSeg = pcb.heapSegment;
    if (heapSeg.startPage == -1 || vAddr.pageNumber < heapSeg.startPage ||
        vAddr.pageNumber >= heapSeg.startPage + heapSeg.numPages) {
        std::cerr << "Invalid heap address: 0x" << std::hex << virtualAddress << std::dec << std::endl;
        return;
    }

    int pagesToFree = heapSeg.startPage + heapSeg.numPages - vAddr.pageNumber;

    int startPage = vAddr.pageNumber;
    int pagesFreed = 0;
    for (int i = startPage; i < startPage + pagesToFree; ++i) {
        PageTableEntry &pte = pcb.pageTable[i];
        if (checkFlag(pte.flags, VALID_BIT)) {
            int frameNumber = pte.frameNumber;
            clearFlag(pte.flags, VALID_BIT | READ_BIT | WRITE_BIT);
            physicalMemory[frameNumber].isFree = true;
            pagesFreed++;
        }
    }

    // Update heap segment
    heapSeg.numPages -= pagesFreed;
    if (heapSeg.numPages == 0) {
        heapSeg.startPage = -1;
    }

    std::cout << "Freed " << pagesFreed * PAGE_SIZE << " bytes of heap memory for process " << processID
              << " starting at virtual address: 0x" << std::hex << virtualAddress << std::dec << std::endl;
}


// Accesses memory in a specified segment
void VMSimulator::accessMemory(int processID, unsigned int virtualAddress, const std::string &segment) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return;
    }
    PCB &pcb = it->second;

    VirtualAddress vAddr(virtualAddress, PAGE_SIZE);

    // Check if the virtual address falls within the specified segment
    Segment *seg = nullptr;
    if (segment == "code") {
        seg = &pcb.codeSegment;
    } else if (segment == "stack") {
        seg = &pcb.stackSegment;
    } else if (segment == "heap") {
        seg = &pcb.heapSegment;
    }

    if (seg == nullptr || vAddr.pageNumber < seg->startPage ||
        vAddr.pageNumber >= seg->startPage + seg->numPages) {
        std::cerr << "Access violation in " << segment << " segment at address: 0x"
                  << std::hex << virtualAddress << std::dec << std::endl;
        return;
    }

    PhysicalAddress pAddr = translateAddress(processID, virtualAddress);
    if (pAddr.frameNumber != -1) {
        std::cout << "Accessed " << segment << " at virtual address: 0x"
                  << std::hex << virtualAddress << " -> physical address: Frame "
                  << pAddr.frameNumber << ", Offset " << pAddr.offset << std::dec << std::endl;
    }
}

PhysicalAddress VMSimulator::translateAddress(int processID, int virtualAddress) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return PhysicalAddress(-1, -1);
    }
    PCB &pcb = it->second;

    VirtualAddress vAddr(virtualAddress, PAGE_SIZE);
    int pageNumber = vAddr.pageNumber;
    int offset = vAddr.offset;

    // Check if page number is within bounds
    if (pageNumber < 0 || pageNumber >= NUM_PAGES) {
        std::cerr << "Invalid page number: " << pageNumber << std::endl;
        return PhysicalAddress(-1, -1);
    }

    // TLB Lookup
    for (auto tlbIt = TLB.begin(); tlbIt != TLB.end(); ++tlbIt) {
        if (tlbIt->pageNumber == pageNumber) {
            int frameNumber = tlbIt->frameNumber;

            // Update TLB for LRU
            TLB.splice(TLB.begin(), TLB, tlbIt);

            return PhysicalAddress(frameNumber, offset);
        }
    }

    // Page Table Lookup
    PageTableEntry &pte = pcb.pageTable[pageNumber];
    if (checkFlag(pte.flags, VALID_BIT)) {
        int frameNumber = pte.frameNumber;

        // Add to TLB
        TLB.push_front(TLBEntry(pageNumber, frameNumber));
        if (TLB.size() > TLB_SIZE) {
            TLB.pop_back();
        }

        return PhysicalAddress(frameNumber, offset);
    } else {
        // Page Fault
        std::cerr << "Page fault at virtual address: 0x" << std::hex << virtualAddress
                  << " for process " << processID << std::dec << std::endl;
        return PhysicalAddress(-1, -1);
    }
}

void VMSimulator::printPageTable(int processID) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process ID " << processID << " not found." << std::endl;
        return;
    }
    PCB &pcb = it->second;

    std::cout << "\nPage Table for Process " << processID << ":" << std::endl;
    std::cout << "Page Number\tFrame Number\tValid\tPermissions" << std::endl;
    for (int i = 0; i < NUM_PAGES; ++i) {
        PageTableEntry &pte = pcb.pageTable[i];
        if (checkFlag(pte.flags, VALID_BIT)) {
            std::cout << i << "\t\t" << pte.frameNumber << "\t\t"
                      << checkFlag(pte.flags, VALID_BIT) << "\t";
            std::cout << (checkFlag(pte.flags, READ_BIT) ? "R" : "")
                      << (checkFlag(pte.flags, WRITE_BIT) ? "W" : "")
                      << std::endl;
        }
    }
}

// Displays the mapping of each page from virtual memory to physical memory
void VMSimulator::printMemoryMapping() {
    std::cout << "\nMemory Mapping (Virtual Page -> Physical Frame):" << std::endl;
    std::cout << "Process ID\tVirtual Page\tPhysical Frame" << std::endl;
    for (const auto &processPair : processes) {
        int pid = processPair.first;
        const PCB &pcb = processPair.second;
        for (int i = 0; i < NUM_PAGES; ++i) {
            const PageTableEntry &pte = pcb.pageTable[i];
            if (checkFlag(pte.flags, VALID_BIT)) {
                std::cout << pid << "\t\t" << i << "\t\t" << pte.frameNumber << std::endl;
            }
        }
    }
}

// For accessing processes map in main (optional)
std::unordered_map<int, PCB>& VMSimulator::getProcesses() {
    return processes;
}
