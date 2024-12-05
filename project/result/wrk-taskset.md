## rust_tokio
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   740.09us  752.85us  41.39ms   96.85%
    Req/Sec    15.65k     3.05k   26.35k    77.46%
  3738316 requests in 30.03s, 292.34MB read
  Non-2xx or 3xx responses: 3738316
Requests/sec: 124480.46
Transfer/sec:      9.73MB
```
## go
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     3.27ms   15.49ms 297.02ms   98.23%
    Req/Sec     8.18k     1.98k   24.09k    73.19%
  1943007 requests in 30.06s, 246.45MB read
Requests/sec:  64631.63
Transfer/sec:      8.20MB
```

## cpp_thread_pool_w_counter
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.91ms   27.94ms 258.85ms   95.85%
    Req/Sec     3.43k   845.07     6.92k    86.38%
  789700 requests in 30.10s, 32.38MB read
  Socket errors: connect 0, read 778317, write 11377, timeout 0
Requests/sec:  26233.29
Transfer/sec:      1.08MB
```
## cpp_thread_pool
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.35ms   25.64ms 272.70ms   95.88%
    Req/Sec     3.44k     0.91k   14.36k    87.40%
  797104 requests in 30.09s, 32.69MB read
  Socket errors: connect 0, read 785998, write 11099, timeout 0
Requests/sec:  26490.25
Transfer/sec:      1.09MB
```
## cpp_thread_pool_100
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     6.23ms   24.24ms 251.25ms   95.92%
    Req/Sec     3.18k   766.74     4.91k    89.88%
  739838 requests in 30.09s, 48.68MB read
  Socket errors: connect 0, read 728558, write 11277, timeout 0
Requests/sec:  24586.69
Transfer/sec:      1.62MB
```

## cpp_async
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     9.65ms   18.68ms 315.98ms   98.26%
    Req/Sec     0.91k   417.88    12.20k    85.73%
  209133 requests in 30.10s, 13.76MB read
  Socket errors: connect 0, read 807004, write 5067, timeout 0
Requests/sec:   6948.89
Transfer/sec:    468.24KB
```

## go_simple
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     9.84ms   16.97ms 401.55ms   98.55%
    Req/Sec     0.88k   429.78     5.61k    84.74%
  202166 requests in 30.35s, 13.30MB read
  Socket errors: connect 0, read 732019, write 6698, timeout 0
Requests/sec:   6661.23
Transfer/sec:    448.85KB
```
## go_simple
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.04ms   17.61ms 296.75ms   98.31%
    Req/Sec     0.87k   445.77     6.80k    84.87%
  198804 requests in 30.09s, 13.08MB read
  Socket errors: connect 0, read 719888, write 7385, timeout 0
Requests/sec:   6605.97
Transfer/sec:    445.13KB
```
## go_gin
```
Running 30s test @ http://127.0.0.1:8080
  8 threads and 500 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    10.34ms   19.88ms 357.75ms   98.89%
    Req/Sec   846.41    357.38     6.00k    86.05%
  191658 requests in 30.06s, 12.61MB read
  Socket errors: connect 0, read 734335, write 4792, timeout 0
Requests/sec:   6376.16
Transfer/sec:    429.64KB
```
