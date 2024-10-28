# 1
for the 3 seed we use other than OPT, all other policies have approximately same hitrate. OPT is higher in some cases

# 2
FIFO repeated in ordered sequence that have length of 5+ like 0,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3,4,5
LRU the same
increase the cash size to 6 would have them have identical result with OPT
MRU repeating the last two elements after a sequence of length of 5+ 0,1,2,3,4,5,4,5,4,5,4,5,4,5,4,5,4,5

# 3
LRU MRU FIFO are close to each other in average when feeding random generated sequences

# 4
we can use absolute value from a 0 centered random destribution, which means equal chance for generitng a pair, 
FIFO and LRU are close and close to OPT, MRU not as good as the rest.
CLOCK with one bit close to Rand, as we increase bits, it works better and can be better than LRU
