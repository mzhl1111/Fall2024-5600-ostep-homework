will look at turn-around time and respond time

two running results from zeyu
```
./scheduler -n 6 -Q 1,2,3,4,5 -A 2,4,6,8,20 -b 20 -j "1,0,10;2,0,5;3,19,10;4,14,20"
Priority Allotments:
Priority 0 allotment: 2 | Quantum: 1
Priority 1 allotment: 4 | Quantum: 2
Priority 2 allotment: 6 | Quantum: 3
Priority 3 allotment: 8 | Quantum: 4
Priority 4 allotment: 20 | Quantum: 5
Priority 5 allotment: 20 | Quantum: 5

Initialized Job 1 with startTime 0 and totalTime 10
Job 1 arrived at time 0 and added to queue 0
Initialized Job 2 with startTime 0 and totalTime 5
Job 2 arrived at time 0 and added to queue 0
Job 1 started at time 0 with priority 0 (Quantum: 1, Allotment: 2)
Job 1 used 1 units at priority 0 (Quantum used: 1, Allotment left: 1)
Job 2 started at time 1 with priority 0 (Quantum: 1, Allotment: 2)
Job 2 used 1 units at priority 0 (Quantum used: 1, Allotment left: 1)
Job 1 resumed at time 2 with priority 0 (Quantum: 1, Allotment left: 1)
Job 1 used 1 units at priority 0 (Quantum used: 1, Allotment left: 0)
Job 1 demoted to queue 1 at time 3 (Allotment reset to 4)
Job 2 resumed at time 3 with priority 0 (Quantum: 1, Allotment left: 1)
Job 2 used 1 units at priority 0 (Quantum used: 1, Allotment left: 0)
Job 2 demoted to queue 1 at time 4 (Allotment reset to 4)
Job 1 resumed at time 4 with priority 1 (Quantum: 2, Allotment left: 4)
Job 1 used 2 units at priority 1 (Quantum used: 2, Allotment left: 2)
Job 2 resumed at time 6 with priority 1 (Quantum: 2, Allotment left: 4)
Job 2 used 2 units at priority 1 (Quantum used: 2, Allotment left: 2)
Job 1 resumed at time 8 with priority 1 (Quantum: 2, Allotment left: 2)
Job 1 used 2 units at priority 1 (Quantum used: 2, Allotment left: 0)
Job 1 demoted to queue 2 at time 10 (Allotment reset to 6)
Job 2 resumed at time 10 with priority 1 (Quantum: 2, Allotment left: 2)
Job 2 finished at time 11
Job 1 resumed at time 11 with priority 2 (Quantum: 3, Allotment left: 6)
Initialized Job 4 with startTime 14 and totalTime 20
Job 4 arrived at time 14 and added to queue 0
Job 1 used 3 units at priority 2 (Quantum used: 3, Allotment left: 3)
Initialized Job 4 with startTime 14 and totalTime 20
Job 4 arrived at time 14 and added to queue 0
Job 4 started at time 14 with priority 0 (Quantum: 1, Allotment: 2)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 1)
Job 4 resumed at time 15 with priority 0 (Quantum: 1, Allotment left: 1)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 0)
Job 4 demoted to queue 1 at time 16 (Allotment reset to 4)
Job 4 resumed at time 16 with priority 0 (Quantum: 1, Allotment left: 4)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 3)
Job 4 resumed at time 17 with priority 1 (Quantum: 2, Allotment left: 3)
Initialized Job 3 with startTime 19 and totalTime 10
Job 3 arrived at time 19 and added to queue 0
Job 4 used 2 units at priority 1 (Quantum used: 2, Allotment left: 1)
Initialized Job 3 with startTime 19 and totalTime 10
Job 3 arrived at time 19 and added to queue 0
Job 3 started at time 19 with priority 0 (Quantum: 1, Allotment: 2)
Boosting all jobs to highest priority at time 20
Job 3 used 1 units at priority 0 (Quantum used: 1, Allotment left: 1)
Job 3 resumed at time 20 with priority 0 (Quantum: 1, Allotment left: 1)
Job 3 used 1 units at priority 0 (Quantum used: 1, Allotment left: 0)
Job 3 demoted to queue 1 at time 21 (Allotment reset to 4)
Job 4 resumed at time 21 with priority 0 (Quantum: 1, Allotment left: 2)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 1)
Job 4 resumed at time 22 with priority 0 (Quantum: 1, Allotment left: 1)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 0)
Job 4 demoted to queue 1 at time 23 (Allotment reset to 4)
Job 1 resumed at time 23 with priority 0 (Quantum: 1, Allotment left: 2)
Job 1 finished at time 24
Job 3 resumed at time 24 with priority 0 (Quantum: 1, Allotment left: 4)
Job 3 used 1 units at priority 0 (Quantum used: 1, Allotment left: 3)
Job 4 resumed at time 25 with priority 0 (Quantum: 1, Allotment left: 4)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 3)
Job 3 resumed at time 26 with priority 1 (Quantum: 2, Allotment left: 3)
Job 3 used 2 units at priority 1 (Quantum used: 2, Allotment left: 1)
Job 4 resumed at time 28 with priority 1 (Quantum: 2, Allotment left: 3)
Job 4 used 2 units at priority 1 (Quantum used: 2, Allotment left: 1)
Job 3 resumed at time 30 with priority 1 (Quantum: 2, Allotment left: 1)
Job 3 used 1 units at priority 1 (Quantum used: 1, Allotment left: 0)
Job 3 demoted to queue 2 at time 31 (Allotment reset to 6)
Job 4 resumed at time 31 with priority 1 (Quantum: 2, Allotment left: 1)
Job 4 used 1 units at priority 1 (Quantum used: 1, Allotment left: 0)
Job 4 demoted to queue 2 at time 32 (Allotment reset to 6)
Job 3 resumed at time 32 with priority 1 (Quantum: 2, Allotment left: 6)
Job 3 used 2 units at priority 1 (Quantum used: 2, Allotment left: 4)
Job 4 resumed at time 34 with priority 1 (Quantum: 2, Allotment left: 6)
Job 4 used 2 units at priority 1 (Quantum used: 2, Allotment left: 4)
Job 3 resumed at time 36 with priority 2 (Quantum: 3, Allotment left: 4)
Job 3 finished at time 38
Job 4 resumed at time 38 with priority 2 (Quantum: 3, Allotment left: 4)
Boosting all jobs to highest priority at time 40
Job 4 used 3 units at priority 2 (Quantum used: 3, Allotment left: -1)
Job 4 demoted to queue 1 at time 41 (Allotment reset to 4)
Job 3 resumed at time 41 with priority 0 (Quantum: 1, Allotment left: 2)
Job 4 resumed at time 41 with priority 0 (Quantum: 1, Allotment left: 4)
Job 4 used 1 units at priority 0 (Quantum used: 1, Allotment left: 3)
Job 4 resumed at time 42 with priority 1 (Quantum: 2, Allotment left: 3)
Job 4 used 2 units at priority 1 (Quantum used: 2, Allotment left: 1)
Job 4 resumed at time 44 with priority 1 (Quantum: 2, Allotment left: 1)
Job 4 finished at time 45
Job 4 resumed at time 45 with priority 1 (Quantum: 2, Allotment left: 1)

Simulation completed at time 45

JobID     StartTime      ResponseTime   TurnaroundTime      
1         0              0              24                  
2         0              1              11                  
3         19             0              19                  
4         14             0              31                  

Final statistics:

```

Mine
```
vscode ➜ .../cpp/Fall2024-5600-ostep-homework/code_hw/chapter7 (master) $ ./a.out 
Enter the number of processes: 4
Enter arrival time and total_length for process 1: 0 10
Enter arrival time and total_length for process 2: 0 5
Enter arrival time and total_length for process 3: 19 10
Enter arrival time and total_length for process 4: 14 20
| ARRIVE |  Time 0: Process 1 has arrived and is placed in queue 1.
| ARRIVE |  Time 0: Process 2 has arrived and is placed in queue 1.
| RUN |  Time 0: Process 1 executing in queue 1 for 1 units.
-| RUN |  Time 1: Process 2 executing in queue 1 for 1 units.
--| RUN |  Time 2: Process 1 executing in queue 1 for 1 units.
---| DEMOTE |  Time 3: Process 1 demoted to queue 2.
---| RUN |  Time 3: Process 2 executing in queue 1 for 1 units.
----| DEMOTE |  Time 4: Process 2 demoted to queue 2.
----| RUN |  Time 4: Process 1 executing in queue 2 for 2 units.
------| RUN |  Time 6: Process 2 executing in queue 2 for 2 units.
--------| RUN |  Time 8: Process 1 executing in queue 2 for 2 units.
----------| DEMOTE |  Time 10: Process 1 demoted to queue 3.
----------| RUN |  Time 10: Process 2 executing in queue 2 for 1 units.
-----------| END |  Time 11: Process 2 completed at time 11.
-----------| RUN |  Time 11: Process 1 executing in queue 3 for 3 units.
--------------| ARRIVE |  Time 14: Process 4 has arrived and is placed in queue 1.
--------------| RUN |  Time 14: Process 4 executing in queue 1 for 1 units.
---------------| RUN |  Time 15: Process 4 executing in queue 1 for 1 units.
----------------| DEMOTE |  Time 16: Process 4 demoted to queue 2.
----------------| RUN |  Time 16: Process 4 executing in queue 2 for 2 units.
------------------| RUN |  Time 18: Process 4 executing in queue 2 for 2 units.
--------------------| DEMOTE |  Time 20: Process 4 demoted to queue 3.
--------------------| BOOST |  Time 20: Resetting all processes to the highest priority queue.
--------------------| ARRIVE |  Time 20: Process 3 has arrived and is placed in queue 1.
--------------------| RUN |  Time 20: Process 1 executing in queue 1 for 1 units.
---------------------| END |  Time 21: Process 1 completed at time 21.
---------------------| RUN |  Time 21: Process 4 executing in queue 1 for 1 units.
----------------------| RUN |  Time 22: Process 3 executing in queue 1 for 1 units.
-----------------------| RUN |  Time 23: Process 4 executing in queue 1 for 1 units.
------------------------| DEMOTE |  Time 24: Process 4 demoted to queue 2.
------------------------| RUN |  Time 24: Process 3 executing in queue 1 for 1 units.
-------------------------| DEMOTE |  Time 25: Process 3 demoted to queue 2.
-------------------------| RUN |  Time 25: Process 4 executing in queue 2 for 2 units.
---------------------------| RUN |  Time 27: Process 3 executing in queue 2 for 2 units.
-----------------------------| RUN |  Time 29: Process 4 executing in queue 2 for 2 units.
-------------------------------| DEMOTE |  Time 31: Process 4 demoted to queue 3.
-------------------------------| RUN |  Time 31: Process 3 executing in queue 2 for 2 units.
---------------------------------| DEMOTE |  Time 33: Process 3 demoted to queue 3.
---------------------------------| RUN |  Time 33: Process 4 executing in queue 3 for 3 units.
------------------------------------| RUN |  Time 36: Process 3 executing in queue 3 for 3 units.
---------------------------------------| RUN |  Time 39: Process 4 executing in queue 3 for 3 units.
------------------------------------------| DEMOTE |  Time 42: Process 4 demoted to queue 4.
------------------------------------------| BOOST |  Time 42: Resetting all processes to the highest priority queue.
------------------------------------------| RUN |  Time 42: Process 3 executing in queue 1 for 1 units.
-------------------------------------------| END |  Time 43: Process 3 completed at time 43.
-------------------------------------------| RUN |  Time 43: Process 4 executing in queue 1 for 1 units.
--------------------------------------------| RUN |  Time 44: Process 4 executing in queue 1 for 1 units.
---------------------------------------------| END |  Time 45: Process 4 completed at time 45.
--------------------------------------------- All processes completed by time 45.
```

result compare:
- process 1: turnaround 21 vs 24 , response same
- process 2: all the same
- process 3: turnaround 24 vs 19, response same
- process 4: all the same

key difference, when a new process comes, one put into the first position of a queue, one put into the last position that follows FIFO, thus little bit difference on the response time, resulting to turnaround time.