# 1
loop once, from 0 to -1 to exit

# 2
sequentially move from 3 tp -1, each loops 3 times

# 3
switch between threads, but register is not shared, each loops 3 times form 3 to -1

# 4
1 thread, 0 to 1

# 5
2 thread, but did not interrupt before assign, each loop bm from f3 to -1, final value of 2000 is 6

# 6
seed interrupts before assign thus final value 1
rest final value 2

# 7
1,2 interrupts just before assign, thus final value 1
3 have final value 2

# 8
1 and 2 values are 100, expected them to be more random, could be view as designed miscount 

# 9
final value 1, only modified by thread 2

# 10
dead loop in thread 1 for 100 vs 1000 times, after thread 2 updates value, both exited