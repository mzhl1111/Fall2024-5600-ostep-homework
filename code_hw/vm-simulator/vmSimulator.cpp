#include "vmSimulator.h"

VirtualAddress::VirtualAddress(long long address, int pageSize) {
    pageNumber = address / pageSize;
    offset = address % pageSize;
}

VirtualAddress::VirtualAddress(int startPage, int numPages, int offset) {
    pageNumber = startPage + numPages - 1;
    offset = offset;
}

PhysicalAddress::PhysicalAddress(int frameNum, int offset) : frameNumber(frameNum), offset(offset) {}

Frame::Frame(int num) : frameNumber(num), isFree(true) {}

PageTableEntry::PageTableEntry() : frameNumber(-1), flags(0) {}

TLBEntry::TLBEntry(int pageNum, int frameNum) : pageNumber(pageNum), frameNumber(frameNum) {}

PCB::PCB(int pid, int numPages) : processID(pid), pageTablePtr(numPages) {
    codeSegment = { -1, 0 , 0, (int)false};
    stackSegment = { -1, 0 , 0, (int)true};
    heapSegment = { -1, 0, 0, (int)false};
}

PCB::PCB() {
    processID = 0;
    pageTablePtr = {};
    codeSegment = { -1, 0 , 0, (int)false};
    stackSegment = { -1, 0 , 0, (int)true};
    heapSegment = { -1, 0, 0, (int)false};
}

VMSimulator::VMSimulator(int pageSize, long long virtualMemSize, long long physicalMemSize, int tlbSize)
    : PAGE_SIZE(pageSize),
      VIRTUAL_MEMORY_SIZE(virtualMemSize),
      PHYSICAL_MEMORY_SIZE(physicalMemSize),
      TLB_SIZE(tlbSize)
{
    NUM_PAGES = VIRTUAL_MEMORY_SIZE / PAGE_SIZE + int((VIRTUAL_MEMORY_SIZE % PAGE_SIZE) > 0);
    NUM_FRAMES = PHYSICAL_MEMORY_SIZE / PAGE_SIZE;
    FreeFrames = NUM_FRAMES;
    stat = {};
    initializePhysicalMemory();
}

void VMSimulator::initializePhysicalMemory() {
    for (int i = 0; i < NUM_FRAMES; ++i) {
        physicalMemory.emplace_back(Frame(i));
    }
}

void VMSimulator::setFlag(unsigned int &flags, unsigned int mask) {
    flags |= mask;
}

void VMSimulator::clearFlag(unsigned int &flags, unsigned int mask) {
    flags &= ~mask;
}

bool VMSimulator::checkFlag(unsigned int flags, unsigned int mask) {
    return (flags & mask) != 0;
}

bool VMSimulator::createProcess(int pid) {
    if (FreeFrames <= (VMSimulator::CODE_SIZE + VMSimulator::STACK_SIZE) / PAGE_SIZE) {
        return false;
    }
    this->processes.emplace(pid, PCB(pid, NUM_PAGES));
    std::cout << "Created process with PID: " << pid << std::endl;

    if (!this->allocateCodeSegment(processes[pid], VMSimulator::CODE_SIZE)) {
        return false;
    }
    if (!this->allocateStackSegment(processes[pid], VMSimulator::STACK_SIZE)) {
        return false;
    }
    stat[pid] = Statistics();
    return true;
}

void VMSimulator::contextSwitch() {
    TLB.clear();
}

int VMSimulator::allocatePages(PCB &pcb, int numPagesNeeded, unsigned int protectionFlags, bool reversed) {
    // Best Fit
    int bestStartFrame = -1;
    int bestSize = NUM_FRAMES + 1;

    int currentStart = -1;
    int currentSize = 0;
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

    if (currentSize >= numPagesNeeded && currentSize < bestSize) {
        bestStartFrame = currentStart;
        bestSize = currentSize;
    }

    if (bestStartFrame == -1) {
        std::cerr << "Not enough physical memory to allocate." << std::endl;
        return -1;
    }

    int startPage = -1;
    int pagesAllocated = 0;
    if (reversed) {
        for (int i = NUM_PAGES-1; i >=0 && pagesAllocated < numPagesNeeded; i--) {
            if (!checkFlag(pcb.pageTablePtr[i].flags, VALID_BIT)) {
                if (startPage == -1) startPage = i;
                pcb.pageTablePtr[i].frameNumber = bestStartFrame + pagesAllocated;
                setFlag(pcb.pageTablePtr[i].flags, VALID_BIT | protectionFlags);
                physicalMemory[bestStartFrame + pagesAllocated].isFree = false;
                pagesAllocated++;
            }        
        } 
    }else {
        for (int i = 0; i < NUM_PAGES && pagesAllocated < numPagesNeeded; i++) {
            if (!checkFlag(pcb.pageTablePtr[i].flags, VALID_BIT)) {
                if (startPage == -1) startPage = i;
                pcb.pageTablePtr[i].frameNumber = bestStartFrame + pagesAllocated;
                setFlag(pcb.pageTablePtr[i].flags, VALID_BIT | protectionFlags);
                physicalMemory[bestStartFrame + pagesAllocated].isFree = false;
                pagesAllocated++;
        }
    }
    }

    if (pagesAllocated < numPagesNeeded) {
        std::cerr << "Not enough virtual pages available." << std::endl;
        return -1;
    }

    this->FreeFrames -= pagesAllocated;
    return startPage;
}


bool VMSimulator::allocateCodeSegment(PCB &pcb, int size) {
    int numPagesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int startPage = allocatePages(pcb, numPagesNeeded, READ_BIT, false);

    if (startPage != -1) {
        pcb.codeSegment = { startPage, numPagesNeeded };
        std::cout << "Allocated code segment for process " << pcb.processID << std::endl;
    } else {
        std::cerr << "Failed to allocate code segment for process " << pcb.processID << std::endl;
        return false;
    }
    return true;
}

bool VMSimulator::allocateStackSegment(PCB &pcb, int size) {
    int numPagesNeeded = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int startPage = allocatePages(pcb, numPagesNeeded, READ_BIT | WRITE_BIT, true);

    if (startPage != -1) {
        pcb.stackSegment = { startPage, numPagesNeeded, true};
        std::cout << "Allocated stack segment for process " << pcb.processID << std::endl;
    } else {
        std::cerr << "Failed to allocate stack segment for process " << pcb.processID << std::endl;
        return false;
    }
    return true;
}

bool VMSimulator::allocateHeapMemory(int processID, unsigned int size) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return false;
    }
    PCB &pcb = it->second;

    int numPagesNeeded = (pcb.heapSegment.offset + size) / PAGE_SIZE;
    int newOffset = (pcb.heapSegment.offset + size) % PAGE_SIZE;
    if (pcb.heapSegment.startPage == -1) numPagesNeeded = std::max(1, numPagesNeeded);
    int startPage = numPagesNeeded > 0 ? allocatePages(pcb, numPagesNeeded, READ_BIT | WRITE_BIT, false): pcb.heapSegment.startPage;

    if (startPage != -1) {
        if (pcb.heapSegment.startPage == -1) {
            pcb.heapSegment = { startPage, numPagesNeeded, newOffset, int(false)};
        } else {
            pcb.heapSegment.numPages += numPagesNeeded;
            pcb.heapSegment.offset = newOffset;
        }
        std::cout << "Allocated " << size << " bytes to heap for process " << processID << std::endl;
    } else {
        std::cerr << "Failed to allocate heap memory for process " << processID << std::endl;
        return false;
    }
    return true;
}

bool VMSimulator::freeHeapMemory(int processID, unsigned int size) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return false;
    }
    PCB &pcb = it->second;

    VirtualAddress vAddr(pcb.heapSegment.startPage, pcb.heapSegment.numPages, pcb.heapSegment.offset);


    Segment &heapSeg = pcb.heapSegment;
    int pagesToFree = 0;
    int c = heapSeg.offset;
    while (c < size) c += PAGE_SIZE, pagesToFree++;
    int newOffset = c - size;

    int startPage = vAddr.pageNumber;
    int pagesFreed = 0;
    for (int i = startPage; i > startPage - pagesToFree; i--) {
        PageTableEntry &pte = pcb.pageTablePtr[i];
        if (checkFlag(pte.flags, VALID_BIT)) {
            int frameNumber = pte.frameNumber;
            clearFlag(pte.flags, VALID_BIT | READ_BIT | WRITE_BIT);
            physicalMemory[frameNumber].isFree = true;
            pagesFreed++;
        }
    }

    heapSeg.numPages -= pagesFreed;
    if (heapSeg.numPages == 0) {
        heapSeg.startPage = -1;
    }
    heapSeg.offset = newOffset;

    std::cout << "Freed " << pagesFreed * PAGE_SIZE << " bytes of heap memory for process " << processID << std::endl;
    return true;
}


bool VMSimulator::accessMemory(int processID, long long virtualAddress, const std::string &segment) {
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return false;
    }
    PCB &pcb = it->second;

    VirtualAddress vAddr(virtualAddress, PAGE_SIZE);

    Segment *seg = nullptr;
    if (segment == "code") {
        seg = &pcb.codeSegment;
    } else if (segment == "stack") {
        seg = &pcb.stackSegment;
    } else if (segment == "heap") {
        seg = &pcb.heapSegment;
    }
    
    if (seg == nullptr) {
        std::cerr << "Access violation in " << segment << " segment at address: 0x"
                  << std::hex << virtualAddress << std::dec << std::endl;
        return false;
    }

    if (!seg->reverse && (vAddr.pageNumber < seg->startPage || vAddr.pageNumber >= seg->startPage + seg->numPages)) {
        std::cerr << "Access violation in " << segment << " segment at address: 0x"
                  << std::hex << virtualAddress << std::dec << std::endl;
        return false;
    }

    if (seg->reverse && (vAddr.pageNumber > seg->startPage || vAddr.pageNumber <= seg->startPage - seg->numPages)) {
                std::cerr << "Access violation in " << segment << " segment at address: 0x"
                  << std::hex << virtualAddress << std::dec << std::endl;
        return false;
    }

    PhysicalAddress pAddr = translateAddress(processID, virtualAddress);
    if (pAddr.frameNumber != -1) {
        // std::cout << "Accessed " << segment << " at virtual address: 0x"
        //           << std::hex << virtualAddress << " -> physical address: Frame "
        //           << pAddr.frameNumber << ", Offset " << pAddr.offset << std::dec << std::endl;
    }
    return true;
}

PhysicalAddress VMSimulator::translateAddress(int processID, long long virtualAddress) {
    stat[processID].n_translate++;
    auto it = processes.find(processID);
    if (it == processes.end()) {
        std::cerr << "Process " << processID << " not found." << std::endl;
        return PhysicalAddress(-1, -1);
    }
    PCB &pcb = it->second;

    VirtualAddress vAddr(virtualAddress, PAGE_SIZE);
    int pageNumber = vAddr.pageNumber;
    int offset = vAddr.offset;

    if (pageNumber < 0 || pageNumber >= NUM_PAGES) {
        std::cerr << "Invalid page number: " << pageNumber << std::endl;
        return PhysicalAddress(-1, -1);
    }

    for (auto tlbIt = TLB.begin(); tlbIt != TLB.end(); ++tlbIt) {
        if (tlbIt->pageNumber == pageNumber) {
            stat[processID].n_TLB_hit_count++;
            int frameNumber = tlbIt->frameNumber;
            TLB.splice(TLB.begin(), TLB, tlbIt);

            return PhysicalAddress(frameNumber, offset);
        }
    }

    PageTableEntry &pte = pcb.pageTablePtr[pageNumber];
    if (checkFlag(pte.flags, VALID_BIT)) {
        int frameNumber = pte.frameNumber;

        TLB.push_front(TLBEntry(pageNumber, frameNumber));
        if (TLB.size() > TLB_SIZE) {
            TLB.pop_back();
        }

        return PhysicalAddress(frameNumber, offset);
    } else {
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
        PageTableEntry &pte = pcb.pageTablePtr[i];
        if (checkFlag(pte.flags, VALID_BIT)) {
            std::cout << i << "\t\t" << pte.frameNumber << "\t\t"
                      << checkFlag(pte.flags, VALID_BIT) << "\t";
            std::cout << (checkFlag(pte.flags, READ_BIT) ? "R" : "")
                      << (checkFlag(pte.flags, WRITE_BIT) ? "W" : "")
                      << std::endl;
        }
    }
}

void VMSimulator::printMemoryMapping() {
    std::cout << "\nMemory Mapping (Virtual Page -> Physical Frame):" << std::endl;
    std::cout << "Process ID\tVirtual Page\tPhysical Frame" << std::endl;
    for (const auto &processPair : processes) {
        int pid = processPair.first;
        const PCB &pcb = processPair.second;
        for (int i = 0; i < NUM_PAGES; ++i) {p
            const PageTableEntry &pte = pcb.pageTablePtr[i];
            if (checkFlag(pte.flags, VALID_BIT)) {
                std::cout << pid << "\t\t" << i << "\t\t" << pte.frameNumber << std::endl;
            }
        }
    }
}

void VMSimulator::printStat(){
    std::cout << "Process ID\t\\# translate\t\\# TLB hit\t TLB Hit Rate" << std::endl;
    for (const auto &statPair : stat) {
        int pid = statPair.first;
        Statistics s = statPair.second;
        float rate = (float)s.n_TLB_hit_count / (float)s.n_translate;
        std::cout << pid << "\t\t" << s.n_translate << "\t\t" << s.n_TLB_hit_count;
        std::cout <<"\t\t" << std::setprecision(4) << rate << std::endl;
    }
}

std::unordered_map<int, PCB>& VMSimulator::getProcesses() {
    return processes;
}
