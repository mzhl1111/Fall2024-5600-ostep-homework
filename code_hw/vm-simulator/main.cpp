#include "vmSimulator.h"
#include <string>
#include <sstream>

void processCommands(VMSimulator &simulator) {
    std::string line;
    std::cin.ignore(); // Ignore any leftover input

    std::cout << "\nEnter commands (type 'exit' to quit):" << std::endl;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (line == "exit") break;

        std::istringstream iss(line);
        int processID;
        std::string command;
        std::string addressStr;
        iss >> processID >> command >> addressStr;

        // Remove '0x' prefix if present and convert address
        unsigned int address = 0;
        if (!addressStr.empty()) {
            if (addressStr.find("0x") == 0 || addressStr.find("0X") == 0) {
                address = std::stoul(addressStr, nullptr, 16);
            } else {
                address = std::stoul(addressStr);
            }
        }

        if (command == "switch") {
            // Handle process switch if needed
            std::cout << "Switching to process " << processID << std::endl;
        } else if (command == "access_stack") {
            simulator.accessMemory(processID, address, "stack");
        } else if (command == "access_code") {
            simulator.accessMemory(processID, address, "code");
        } else if (command == "alloc") {
            simulator.allocateHeapMemory(processID, address);
        } else if (command == "free") {
            simulator.freeHeapMemory(processID, address);
        } else if (command == "access_heap") {
            simulator.accessMemory(processID, address, "heap");
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
}

int main() {
    int pageSize, virtualMemSize, physicalMemSize, tlbSize;

    // Prompt user for system parameters
    std::cout << "Enter page size (bytes): ";
    std::cin >> pageSize;
    std::cout << "Enter virtual memory size (bytes): ";
    std::cin >> virtualMemSize;
    std::cout << "Enter physical memory size (bytes): ";
    std::cin >> physicalMemSize;
    std::cout << "Enter TLB size (number of entries): ";
    std::cin >> tlbSize;

    // Create VMSimulator instance with user-defined parameters
    VMSimulator simulator(pageSize, virtualMemSize, physicalMemSize, tlbSize);

    // Create initial process
    int pid = simulator.createProcess();

    // Allocate segments for the process
    simulator.allocateCodeSegment(simulator.getProcesses()[pid], /* code size */ 1024);
    simulator.allocateStackSegment(simulator.getProcesses()[pid], /* stack size */ 1024);
    simulator.allocateHeapSegment(simulator.getProcesses()[pid], /* initial heap size */ 1024);


    simulator.printPageTable(pid);
    simulator.printMemoryMapping();

    // Process commands
    processCommands(simulator);

    return 0;
}
