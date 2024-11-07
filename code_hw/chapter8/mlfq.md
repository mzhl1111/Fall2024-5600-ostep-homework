# 1
```
python mlfq.py -l 0,10,0:2,20,0
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 3
OPTIONS allotments for queue  2 is   1
OPTIONS quantum length for queue  2 is  10
OPTIONS allotments for queue  1 is   1
OPTIONS quantum length for queue  1 is  10
OPTIONS allotments for queue  0 is   1
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False
```

# 2
python mlfq.py -l 0,200,0 -n 3 -q 10 -a 10
python mlfq.py -l 0,180,0:100,20,0 -n 3 -q 10 -a 10
python mlfq.py -l 0,180,0:100,20,0 -n 3 -q 1 -a 10
python mlfq.py -l 0,130,0:100,20,50 -n 3 -q 1 -a 10
python mlfq.py -l 0,150,0:100,20,0 -n 3 -q 10 -a 10
python mlfq.py -l 0,150,0:10,150,0 -n 3 -q 10 -a 10

# 3
1 queue

# 4
python mlfq.py -l 0,130,0:100,20,50 -n 3 -q 1 -a 10 -S

# 5
20 * 10 * (#queue -1)


