#   Q1
- CPU utilization will be 100, as 100% chance to be occupied by instruction. NO IO there.
```
file-devices % python process-run.py -l 5:100,5:100 -p -c
Time    PID: 0    PID: 1       CPU       IOs
  1    RUN:cpu     READY         1          
  2    RUN:cpu     READY         1          
  3    RUN:cpu     READY         1          
  4    RUN:cpu     READY         1          
  5    RUN:cpu     READY         1          
  6       DONE   RUN:cpu         1          
  7       DONE   RUN:cpu         1          
  8       DONE   RUN:cpu         1          
  9       DONE   RUN:cpu         1          
 10       DONE   RUN:cpu         1          

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```
# Q2
- 10, 5 instructions to run, default IO time is 5 
```
file-devices % python process-run.py -l 4:100,1:0 -c -p
Time    PID: 0    PID: 1       CPU       IOs
  1    RUN:cpu     READY         1          
  2    RUN:cpu     READY         1          
  3    RUN:cpu     READY         1          
  4    RUN:cpu     READY         1          
  5       DONE    RUN:io         1          
  6       DONE   WAITING                   1
  7       DONE   WAITING                   1
  8       DONE   WAITING                   1
  9       DONE   WAITING                   1
 10*      DONE      DONE                    

Stats: Total Time 10
Stats: CPU Busy 5 (50.00%)
Stats: IO Busy  4 (40.00%)
```

# Q3
- 6, IO will not block instruction but default takes 5 unit of time finish.
```
file-devices % python process-run.py -l 1:0,4:100 -c -p 
Time    PID: 0    PID: 1       CPU       IOs
  1     RUN:io     READY         1          
  2    WAITING   RUN:cpu         1         1
  3    WAITING   RUN:cpu         1         1
  4    WAITING   RUN:cpu         1         1
  5    WAITING   RUN:cpu         1         1
  6*      DONE      DONE                    

Stats: Total Time 6
Stats: CPU Busy 5 (83.33%)
Stats: IO Busy  4 (66.67%)
```

# Q4
- 9, IO takes 5 and then for the 4 instructions
```
ile-devices % python process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END -p
Time    PID: 0    PID: 1       CPU       IOs
  1     RUN:io     READY         1          
  2    WAITING     READY                   1
  3    WAITING     READY                   1
  4    WAITING     READY                   1
  5    WAITING     READY                   1
  6*      DONE   RUN:cpu         1          
  7       DONE   RUN:cpu         1          
  8       DONE   RUN:cpu         1          
  9       DONE   RUN:cpu         1          

Stats: Total Time 9
Stats: CPU Busy 5 (55.56%)
Stats: IO Busy  4 (44.44%)
```

# Q5
- same as Q3
```
file-devices % python process-run.py -l 1:0,4:100 -c -S SWITCH_ON_IO -p
Time    PID: 0    PID: 1       CPU       IOs
  1     RUN:io     READY         1          
  2    WAITING   RUN:cpu         1         1
  3    WAITING   RUN:cpu         1         1
  4    WAITING   RUN:cpu         1         1
  5    WAITING   RUN:cpu         1         1
  6*      DONE      DONE                    

Stats: Total Time 6
Stats: CPU Busy 5 (83.33%)
Stats: IO Busy  4 (66.67%)
```

# Q6
- No, waiting time and CPU busy time only overlaps 6/15
```
file-devices % python process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -c -p -I IO_RUN_LATER
Time    PID: 0    PID: 1    PID: 2    PID: 3       CPU       IOs
  1     RUN:io     READY     READY     READY         1          
  2    WAITING   RUN:cpu     READY     READY         1         1
  3    WAITING   RUN:cpu     READY     READY         1         1
  4    WAITING   RUN:cpu     READY     READY         1         1
  5    WAITING   RUN:cpu     READY     READY         1         1
  6*     READY   RUN:cpu     READY     READY         1          
  7      READY      DONE   RUN:cpu     READY         1          
  8      READY      DONE   RUN:cpu     READY         1          
  9      READY      DONE   RUN:cpu     READY         1          
 10      READY      DONE   RUN:cpu     READY         1          
 11      READY      DONE   RUN:cpu     READY         1          
 12      READY      DONE      DONE   RUN:cpu         1          
 13      READY      DONE      DONE   RUN:cpu         1          
 14      READY      DONE      DONE   RUN:cpu         1          
 15      READY      DONE      DONE   RUN:cpu         1          
 16      READY      DONE      DONE   RUN:cpu         1          
 17     RUN:io      DONE      DONE      DONE         1          
 18    WAITING      DONE      DONE      DONE                   1
 19    WAITING      DONE      DONE      DONE                   1
 20    WAITING      DONE      DONE      DONE                   1
 21    WAITING      DONE      DONE      DONE                   1
 22*    RUN:io      DONE      DONE      DONE         1          
 23    WAITING      DONE      DONE      DONE                   1
 24    WAITING      DONE      DONE      DONE                   1
 25    WAITING      DONE      DONE      DONE                   1
 26    WAITING      DONE      DONE      DONE                   1
 27*      DONE      DONE      DONE      DONE                    

Stats: Total Time 27
Stats: CPU Busy 18 (66.67%)
Stats: IO Busy  12 (44.44%)
```

# Q7
- 9 unit time faster then Q6, all waiting time overlaps with CPU busy time for pid 0 utilized.
```
file-devices % python process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -c -p -I IO_RUN_IMMEDIATE
Time    PID: 0    PID: 1    PID: 2    PID: 3       CPU       IOs
  1     RUN:io     READY     READY     READY         1          
  2    WAITING   RUN:cpu     READY     READY         1         1
  3    WAITING   RUN:cpu     READY     READY         1         1
  4    WAITING   RUN:cpu     READY     READY         1         1
  5    WAITING   RUN:cpu     READY     READY         1         1
  6*    RUN:io     READY     READY     READY         1          
  7    WAITING   RUN:cpu     READY     READY         1         1
  8    WAITING      DONE   RUN:cpu     READY         1         1
  9    WAITING      DONE   RUN:cpu     READY         1         1
 10    WAITING      DONE   RUN:cpu     READY         1         1
 11*    RUN:io      DONE     READY     READY         1          
 12    WAITING      DONE   RUN:cpu     READY         1         1
 13    WAITING      DONE   RUN:cpu     READY         1         1
 14    WAITING      DONE      DONE   RUN:cpu         1         1
 15    WAITING      DONE      DONE   RUN:cpu         1         1
 16*      DONE      DONE      DONE   RUN:cpu         1          
 17       DONE      DONE      DONE   RUN:cpu         1          
 18       DONE      DONE      DONE   RUN:cpu         1          

Stats: Total Time 18
Stats: CPU Busy 18 (100.00%)
Stats: IO Busy  12 (66.67%)
```

#  Q8
- `IO_RUN` not change a lot the result
- `SWITCH_ON_END` always increase the total runtime a lot