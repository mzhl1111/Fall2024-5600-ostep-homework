#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

#define RESET   "\033[0m"
#define DEMOTE     "\033[31m"      
#define ARRIVE   "\033[32m"      
#define BOOST  "\033[33m"      
#define RUN    "\033[34m"      
#define END "\033[35m"          


struct Process {
    int pid;
    int arrival_time;
    int total_length;
    int remaining_time;
    int current_queue;
    int total_time_in_level;
    int last_executed_time;
};

bool arrivalTimeComparator(const Process& a, const Process& b) {
    return a.arrival_time < b.arrival_time;
}

int main() {
    const int NUM_QUEUES = 4;
    const int time_quantum[NUM_QUEUES] = {4, 8, 12, 100};
    const int time_allotment[NUM_QUEUES] = {8, 16, 48, INT_MAX}; 
    const int RESET_PERIOD = 50;

    string padding = "";

    int num_processes;
    cout << "Enter the number of processes: ";
    cin >> num_processes;

    vector<Process> processes(num_processes);

    for (int i = 0; i < num_processes; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time and total_length for process " << processes[i].pid << ": ";
        cin >> processes[i].arrival_time >> processes[i].total_length;
        processes[i].remaining_time = processes[i].total_length;
        processes[i].current_queue = 0; 
        processes[i].total_time_in_level = 0;
        processes[i].last_executed_time = processes[i].arrival_time;
    }

    sort(processes.begin(), processes.end(), arrivalTimeComparator);

    vector<queue<Process*>> queues(NUM_QUEUES);

    int time = 0;
    int completed = 0;
    vector<Process*> arrived_processes;

    int time_since_reset = 0;

    while (completed < num_processes) {
        if (time_since_reset >= RESET_PERIOD) {
            cout << padding << BOOST << "| BOOST |  " << RESET << "Time " << time << ": Resetting all processes to the highest priority queue.\n";
            for (int i = 1; i < NUM_QUEUES; ++i) {
                while (!queues[i].empty()) {
                    Process* proc = queues[i].front();
                    queues[i].pop();
                    proc->current_queue = 0;
                    proc->total_time_in_level = 0;
                    queues[0].push(proc);
                }
            }
            time_since_reset = 0;
        }

        for (auto& proc : processes) {
            if (proc.arrival_time <= time && proc.remaining_time > 0 && find(arrived_processes.begin(), arrived_processes.end(), &proc) == arrived_processes.end()) {
                queues[0].push(&proc);
                arrived_processes.push_back(&proc);
                cout << padding << ARRIVE << "| ARRIVE |  " << RESET << "Time " << time << ": Process " << proc.pid << " has arrived and is placed in queue 1.\n";
            }
        }

        bool executed = false;

        for (int i = 0; i < NUM_QUEUES; ++i) {
            if (!queues[i].empty()) {
                Process* proc = queues[i].front();
                queues[i].pop();

                int tq = time_quantum[i];
                int exec_time = min(tq, proc->remaining_time);

                cout << padding  << RUN << "| RUN |  " << RESET << "Time " << time << ": Process " << proc->pid << " executing in queue " << i + 1 << " for " << exec_time << " units.\n";

                time += exec_time;
                time_since_reset += exec_time;
                proc->remaining_time -= exec_time;
                proc->total_time_in_level += exec_time;
                proc->last_executed_time = time;
                for (int i = 0; i < exec_time; i++) padding += "-";
                executed = true;

                bool demote = false;
                if (time_allotment[i] > 0 && proc->total_time_in_level >= time_allotment[i]) {
                    demote = true;
                    proc->total_time_in_level = 0; // Reset time in level
                }

                if (proc->remaining_time > 0) {
                    if (demote && i < NUM_QUEUES - 1) {
                        proc->current_queue = i + 1; // Demote to next queue
                        queues[i + 1].push(proc);
                        cout << padding << DEMOTE << "| DEMOTE |  " << RESET << "Time " << time << ": Process " << proc->pid << " demoted to queue " << i + 2 << ".\n";
                    } else {
                        queues[i].push(proc); // Stay in the same queue
                    }
                } else {
                    completed++;
                    cout << padding  << END << "| END |  " << RESET << "Time " << time << ": Process " << proc->pid << " completed at time " << time << ".\n";
                }
                break;
            }
        }

        if (!executed) {
            time++;
            time_since_reset++;
            padding += "-";
        }
    }

    cout << padding << " All processes completed by time " << time << ".\n";

    return 0;
}
