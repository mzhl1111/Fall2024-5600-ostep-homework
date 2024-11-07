# Read Me

## Config
Config can be changed accordingly
- NUM_QUEUES : number of queues
- time_quantum[NUM_QUEUES]: time quantum for each queue
- time_allotment[NUM_QUEUES]: max allowed time for each queue
- RESET_PERIOD = 50: time interval between boosting

run `g++ mlfq_simiulator.cpp` to re-compile and run

## How to use
Prompt line will guild you input the number of process
Then each process's start time and length, split by space

## Example Runs
```
Enter the number of processes: 3
Enter arrival time and total_length for process 1: 0 10
Enter arrival time and total_length for process 2: 2 20
Enter arrival time and total_length for process 3: 50 50
| ARRIVE |  Time 0: Process 1 has arrived and is placed in queue 1.
| RUN |  Time 0: Process 1 executing in queue 1 for 4 units.
----| ARRIVE |  Time 4: Process 2 has arrived and is placed in queue 1.
----| RUN |  Time 4: Process 1 executing in queue 1 for 4 units.
--------| DEMOTE |  Time 8: Process 1 demoted to queue 2.
--------| RUN |  Time 8: Process 2 executing in queue 1 for 4 units.
------------| RUN |  Time 12: Process 2 executing in queue 1 for 4 units.
----------------| DEMOTE |  Time 16: Process 2 demoted to queue 2.
----------------| RUN |  Time 16: Process 1 executing in queue 2 for 2 units.
------------------| END |  Time 18: Process 1 completed at time 18.
------------------| RUN |  Time 18: Process 2 executing in queue 2 for 8 units.
--------------------------| RUN |  Time 26: Process 2 executing in queue 2 for 4 units.
------------------------------| END |  Time 30: Process 2 completed at time 30.
--------------------------------------------------| BOOST |  Time 50: Resetting all processes to the highest priority queue.
--------------------------------------------------| ARRIVE |  Time 50: Process 3 has arrived and is placed in queue 1.
--------------------------------------------------| RUN |  Time 50: Process 3 executing in queue 1 for 4 units.
------------------------------------------------------| RUN |  Time 54: Process 3 executing in queue 1 for 4 units.
----------------------------------------------------------| DEMOTE |  Time 58: Process 3 demoted to queue 2.
----------------------------------------------------------| RUN |  Time 58: Process 3 executing in queue 2 for 8 units.
------------------------------------------------------------------| RUN |  Time 66: Process 3 executing in queue 2 for 8 units.
--------------------------------------------------------------------------| DEMOTE |  Time 74: Process 3 demoted to queue 3.
--------------------------------------------------------------------------| RUN |  Time 74: Process 3 executing in queue 3 for 12 units.
--------------------------------------------------------------------------------------| RUN |  Time 86: Process 3 executing in queue 3 for 12 units.
--------------------------------------------------------------------------------------------------| RUN |  Time 98: Process 3 executing in queue 3 for 2 units.
----------------------------------------------------------------------------------------------------| END |  Time 100: Process 3 completed at time 100.
---------------------------------------------------------------------------------------------------- All processes completed by time 100.
```

