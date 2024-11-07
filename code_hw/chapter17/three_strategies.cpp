#include <iostream>
#include <list>
#include <string>
#include "three_strategies.h"

using namespace std;

MemoryManager::MemoryManager(int size, const string& strat)
    : totalSize(size), lastId(0), strategy(strat),
      allocationCount(0), deallocationCount(0), allocationFailures(0) {
    Block initialBlock = {0, size, 0, true};
    memoryList.push_back(initialBlock);
}

bool MemoryManager::allocate(int size) {
    bool success = false;
    if (strategy == "first") {
        success = firstFit(size);
    } else if (strategy == "best") {
        success = bestFit(size);
    } else if (strategy == "worst") {
        success = worstFit(size);
    } else {
        std::cout << "not implemented!" << endl;
        return false;
    }

    if (success) {
        allocationCount++;
    } else {
        allocationFailures++;
    }
    return success;
}

bool MemoryManager::deallocate(int id) {
    for (auto it = memoryList.begin(); it != memoryList.end(); ++it) {
        if (it->id == id) {
            it->free = true;
            it->id = 0;
            deallocationCount++;

            if (it != memoryList.begin()) {
                auto prevIt = prev(it);
                if (prevIt->free) {
                    prevIt->size += it->size;
                    memoryList.erase(it);
                    it = prevIt;
                }
            }
            auto nextIt = next(it);
            if (nextIt != memoryList.end() && nextIt->free) {
                it->size += nextIt->size;
                memoryList.erase(nextIt);
            }
            return true;
        }
    }
    return false;
}

void MemoryManager::showMetrics() {
    int totalFree = 0;
    int largestFreeBlock = 0;
    int totalAllocated = 0;

    for (const auto& block : memoryList) {
        if (block.free) {
            totalFree += block.size;
            if (block.size > largestFreeBlock) {
                largestFreeBlock = block.size;
            }
        } else {
            totalAllocated += block.size;
        }
    }

    double utilization = (double)totalAllocated / totalSize * 100;

    std::cout << "\n--- Results ---" << endl;
    std::cout << "Memory Used: " << utilization << "%" << endl;
    std::cout << "Free Memory: " << totalFree << endl;
    std::cout << "Largest Free Block: " << largestFreeBlock << endl;
    std::cout << "Total Allocations: " << allocationCount << endl;
    std::cout << "Total Deallocation: " << deallocationCount << endl;
    std::cout << "Allocation Failures: " << allocationFailures << endl;
}

bool MemoryManager::firstFit(int size) {
    for (auto it = memoryList.begin(); it != memoryList.end(); ++it) {
        if (it->free && it->size >= size) {
            int remainingSize = it->size - size;
            it->free = false;
            it->size = size;
            it->id = ++lastId;

            if (remainingSize > 0) {
                Block newBlock = {0, remainingSize, it->start + size, true};
                memoryList.insert(next(it), newBlock);
            }
            return true;
        }
    }
    return false; 
}

bool MemoryManager::bestFit(int size) {
    auto bestIt = memoryList.end();
    int minSize = totalSize + 1;

    for (auto it = memoryList.begin(); it != memoryList.end(); ++it) {
        if (it->free && it->size >= size && it->size < minSize) {
            minSize = it->size;
            bestIt = it;
        }
    }

    if (bestIt != memoryList.end()) {
        int remainingSize = bestIt->size - size;
        bestIt->free = false;
        bestIt->size = size;
        bestIt->id = ++lastId;

        if (remainingSize > 0) {
            Block newBlock = {0, remainingSize, bestIt->start + size, true};
            memoryList.insert(next(bestIt), newBlock);
        }
        return true;
    }
    return false; 
}


bool MemoryManager::worstFit(int size) {
    auto worstIt = memoryList.end();
    int maxSize = -1;

    for (auto it = memoryList.begin(); it != memoryList.end(); ++it) {
        if (it->free && it->size >= size && it->size > maxSize) {
            maxSize = it->size;
            worstIt = it;
        }
    }

    if (worstIt != memoryList.end()) {
        int remainingSize = worstIt->size - size;
        worstIt->free = false;
        worstIt->size = size;
        worstIt->id = ++lastId;

        if (remainingSize > 0) {
            Block newBlock = {0, remainingSize, worstIt->start + size, true};
            memoryList.insert(next(worstIt), newBlock);
        }
        return true;
    }
    return false; 
}
