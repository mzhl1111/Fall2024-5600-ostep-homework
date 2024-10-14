# Compare three strategies

Used random to generate operations, currently set allocation vs deallocation 4:1
most of the time they are the same
in some cases they are different, but 'Best Fit' always looks better

```
% ./ts 1000 20 

Running simulation for strategy: first

--- Results ---
Memory Used: 20%
Free Memory: 800
Largest Free Block: 354
Total Allocations: 5
Total Deallocation: 3
Allocation Failures: 10

Running simulation for strategy: best

--- Results ---
Memory Used: 58.4%
Free Memory: 416
Largest Free Block: 240
Total Allocations: 6
Total Deallocation: 3
Allocation Failures: 9

Running simulation for strategy: worst

--- Results ---
Memory Used: 20%
Free Memory: 800
Largest Free Block: 354
Total Allocations: 5
Total Deallocation: 3
Allocation Failures: 10
```