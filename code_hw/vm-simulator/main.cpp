#include "vmSimulator.h"
#include <string>
#include <cstring>
#include <sstream>
#include <unordered_set>
#include <fstream>


std::unordered_set<int> pidSet = {};

void processCommands(VMSimulator *simulator, std::string &filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Failed to open command file: " << filename << std::endl;
        return;
    }

    std::string line;

    std::cout << "\nProcessing commands from file: " << filename << std::endl;

    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;
        if (line == "exit") break;

        std::istringstream iss(line);
        int processID;
        std::string command;
        std::string addressStr;
        iss >> processID >> command >> addressStr;
        pidSet.insert(processID);

        unsigned int address = 0;
        if (!addressStr.empty()) {
            if (addressStr.find("0x") == 0 || addressStr.find("0X") == 0) {
                address = std::stoul(addressStr, nullptr, 16);
            } else {
                address = std::stoul(addressStr);
            }
        }

        if (command == "switch") {
            std::cout << "Switching to process " << processID << std::endl;
            auto ps = simulator->getProcesses();
            if (ps.find(processID) == ps.end()) {
                simulator->createProcess(processID);
            }
            simulator->contextSwitch();
        } else if (command == "access_stak") {
            simulator->accessMemory(processID, address, "stack");
        } else if (command == "access_code") {
            simulator->accessMemory(processID, address, "code");
        } else if (command == "alloc") {
            simulator->allocateHeapMemory(processID, address);
        } else if (command == "free") {
            simulator->freeHeapMemory(processID, address);
        } else if (command == "access_heap") {
            simulator->accessMemory(processID, address, "heap");
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
}

int main() {
    int pageSize = 4096;
    long long virtualMemSize = (long long)0xffffffff;
    long long physicalMemSize = (virtualMemSize / 2 / pageSize) * pageSize;
    int tlbSize = 50;

    auto simulator = new VMSimulator(pageSize, virtualMemSize, physicalMemSize, tlbSize);

    std::string commandFile = "test_data.txt";
    processCommands(simulator, commandFile);

    for (auto pid : pidSet) {
        simulator->printPageTable(pid);
    }
    simulator->printMemoryMapping();

    return 0;
}
