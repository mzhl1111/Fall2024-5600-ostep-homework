#include "three_strategies.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>

using namespace std;

struct Operation {
    enum Type { ALLOCATE, DEALLOCATE } type;
    int size;  
    int id;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <total_memory_size> <number_of_allocation_tasks>" << endl;
        return 1;
    }

    int totalSize;
    int numOps;

    totalSize = stoi(argv[1]);
    numOps = stoi(argv[2]);


    srand(static_cast<unsigned int>(std::time(nullptr)));

    vector<Operation> opsList;
    vector<int> idList; 

    int currentId = 1;
    for (int i = 0; i < numOps; ++i) {
        Operation op;
        if (!idList.empty() && (rand() % 4 == 0)) {
            op.type = Operation::DEALLOCATE;
            int index = rand() % idList.size();
            op.id = idList[index];
            idList.erase(idList.begin() + index);
        } else {
            op.type = Operation::ALLOCATE;
            op.size = (rand() % totalSize) + 1;
            op.id = currentId++;
            idList.push_back(op.id);
        }
        opsList.push_back(op);
    }

    vector<string> strategies = {"first", "best", "worst"};
    map<string, map<int, int>> strategyIdMap;
    for (const auto& strategy : strategies) {
        cout << "\nRunning simulation for strategy: " << strategy << endl;

        auto manager = new MemoryManager(totalSize, strategy);

        map<int, int> idMap;

        for (const auto& op : opsList) {
            if (op.type == Operation::ALLOCATE) {
                bool success = manager->allocate(op.size);
                if (success) {
                    idMap[op.id] = op.id;
                }
            } else if (op.type == Operation::DEALLOCATE) {
                auto it = idMap.find(op.id);
                if (it != idMap.end()) {
                    manager->deallocate(it->second);
                    idMap.erase(it);
                }
            }
        }
        manager->showMetrics();
        strategyIdMap[strategy] = idMap;

        delete manager;
    }

    return 0;
}
