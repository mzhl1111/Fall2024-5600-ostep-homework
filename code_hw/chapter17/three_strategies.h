#ifndef THREE_H
#define THREE_H

#include <iostream>
#include <list>
#include <string>

using namespace std;

struct Block {
    int id;         
    int size;       
    int start;      
    bool free;      
};

class MemoryManager {
private:
    int totalSize;           
    int lastId;              
    list<Block> memoryList; 
    string strategy;    

    int allocationCount;
    int deallocationCount;
    int allocationFailures;

public:
    MemoryManager(int size, const string& strat);

    bool allocate(int size);
    bool deallocate(int id);
    void displayMemory();
    void showMetrics();

private:

    bool firstFit(int size);
    bool bestFit(int size);
    bool worstFit(int size);
};

#endif
