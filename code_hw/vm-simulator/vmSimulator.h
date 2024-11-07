#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>

const unsigned int VALID_BIT = 0x1;
const unsigned int READ_BIT = 0x2;
const unsigned int WRITE_BIT = 0x4;

struct Statistics {
    int n_translate = 0;
    int n_TLB_hit_count = 0;

    Statistics() = default;
};

struct Segment {
    int startPage;
    int numPages;
    int offset;
    int reverse;
};

struct VirtualAddress {
    int pageNumber;
    int offset;

    VirtualAddress(long long address, int pageSize);
    VirtualAddress(int startPage, int numPage, int offset);
};

struct PhysicalAddress {
    int frameNumber;
    int offset;

    PhysicalAddress(int frameNum, int offset);
};

struct Frame {
    int frameNumber;
    bool isFree;

    Frame(int num);
};

struct PageTableEntry {
    int frameNumber;
    unsigned int flags;

    PageTableEntry();
};

struct TLBEntry {
    int pageNumber;
    int frameNumber;
    // TODO: CONTROL BITS 

    TLBEntry(int pageNum, int frameNum);
};

struct PCB {
    int processID;
    std::vector<PageTableEntry> pageTablePtr; // PCB only have ptr to page table, to simplify, directly store page table info here

    Segment codeSegment;
    Segment stackSegment;
    Segment heapSegment;

    PCB();
    PCB(int pid, int numPages);
};

class VMSimulator {
private:
    int PAGE_SIZE;
    long long VIRTUAL_MEMORY_SIZE;
    long long PHYSICAL_MEMORY_SIZE;
    int NUM_PAGES;
    int NUM_FRAMES;
    int TLB_SIZE;
    Statistics stat;

    std::vector<Frame> physicalMemory;
    std::list<TLBEntry> TLB; // using list to simplify the LRU operation for TLB table
    std::unordered_map<int, PCB> processes;

    int FreeFrames;

    void initializePhysicalMemory();
    int allocatePages(PCB &pcb, int numPagesNeeded, unsigned int protectionFlags, bool reversed);
    void setFlag(unsigned int &flags, unsigned int mask);
    void clearFlag(unsigned int &flags, unsigned int mask);
    bool checkFlag(unsigned int flags, unsigned int mask);
    bool allocateCodeSegment(PCB &pcb, int size);
    bool allocateStackSegment(PCB &pcb, int size);

public:
    inline static int CODE_SIZE = 4194304; // 4 * 1024 * 1024
    inline static int STACK_SIZE = 4194304; 
    VMSimulator(int pageSize, long long virtualMemSize, long long physicalMemSize, int tlbSize);

    bool createProcess(int pid);

    bool allocateHeapMemory(int processID, unsigned int size);
    bool freeHeapMemory(int processID, unsigned int size);
    bool accessMemory(int processID, long long virtualAddress, const std::string &segment);

    PhysicalAddress translateAddress(int processID, long long virtualAddress);
    void contextSwitch();

    void printPageTable(int processID);
    void printMemoryMapping();

    std::unordered_map<int, PCB>& getProcesses();
};


#endif
