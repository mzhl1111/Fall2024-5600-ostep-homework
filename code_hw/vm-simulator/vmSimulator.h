#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>

// Bit masks for flags
const unsigned int VALID_BIT = 0x1;
const unsigned int READ_BIT = 0x2;
const unsigned int WRITE_BIT = 0x4;

// Address Structures
struct VirtualAddress {
    int pageNumber;
    int offset;

    VirtualAddress(int address, int pageSize);
};

struct PhysicalAddress {
    int frameNumber;
    int offset;

    PhysicalAddress(int frameNum, int offset);
};

// Frame Structure
struct Frame {
    int frameNumber;
    bool isFree;

    Frame(int num);
};

// Page Table Entry Structure with Bit Manipulation
struct PageTableEntry {
    int frameNumber;
    unsigned int flags; // Valid and protection bits

    PageTableEntry();
};

// TLB Entry Structure
struct TLBEntry {
    int pageNumber;
    int frameNumber;

    TLBEntry(int pageNum, int frameNum);
};

// Segment Structure
struct Segment {
    int startPage;
    int numPages;
};

// Process Control Block (PCB)
struct PCB {
    int processID;
    std::vector<PageTableEntry> pageTable;

    Segment codeSegment;
    Segment stackSegment;
    Segment heapSegment;

    PCB(int pid, int numPages);
};

class VMSimulator {
private:
    int PAGE_SIZE;
    int VIRTUAL_MEMORY_SIZE;
    int PHYSICAL_MEMORY_SIZE;
    int NUM_PAGES;
    int NUM_FRAMES;
    int TLB_SIZE;

    std::vector<Frame> physicalMemory;
    std::list<TLBEntry> TLB;
    std::unordered_map<int, PCB> processes;
    int nextProcessID;

    // Helper methods
    void initializePhysicalMemory();
    int allocatePages(PCB &pcb, int numPagesNeeded, unsigned int protectionFlags);
    void setFlag(unsigned int &flags, unsigned int mask);
    void clearFlag(unsigned int &flags, unsigned int mask);
    bool checkFlag(unsigned int flags, unsigned int mask);

public:
    VMSimulator(int pageSize, int virtualMemSize, int physicalMemSize, int tlbSize);

    int createProcess();
    void allocateCodeSegment(PCB &pcb, int size);
    void allocateStackSegment(PCB &pcb, int size);
    void allocateHeapSegment(PCB &pcb, int size);

    bool allocateHeapMemory(int processID, unsigned int size);
    bool freeHeapMemory(int processID, unsigned int virtualAddress);
    bool accessMemory(int processID, unsigned int virtualAddress, const std::string &segment);

    void freeMemory(int processID, int virtualAddress, int size);
    PhysicalAddress translateAddress(int processID, int virtualAddress);

    void printPageTable(int processID);
    void printMemoryMapping();

    // For accessing processes map in main (optional)
    std::unordered_map<int, PCB>& getProcesses();
};

#endif
