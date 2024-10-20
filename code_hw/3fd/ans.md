## Comparative Summary
- Integration with Event Mechanisms: All three system calls allow events (signals, process changes, page faults) to be integrated into standard I/O event loops using file descriptors.

- Event Handling Simplification: They simplify the handling of complex system events by providing a unified interface, reducing the need for separate mechanisms like signal handlers or special process management code.

## Use Cases:

- signalfd is ideal for applications that need to handle signals in a non-disruptive way within an event-driven architecture.
- pidfd is crucial for applications that require reliable process management, such as supervising daemons or handling child processes in multi-threaded environments.
- userfaultfd is suited for advanced applications that need to manage memory dynamically, such as hypervisors, checkpointing tools, or systems requiring custom paging strategies.
## Complexity:

signalfd and pidfd are relatively straightforward to use and integrate into existing applications.
userfaultfd is more complex due to the intricacies of memory management:
- page faults occur only on first access, once the page fault is handled—by bringing the page into memory and updating the page tables—the faulting thread (in this case, the main process) continues execution as if the page had been there all along.After the page is populated and mapped correctly, subsequent accesses to that page by the main process won't cause additional page faults

## Test runs
In my test run, we reset all pages as arry of char with all 'a's and then re-access the memory later when all page faults triggered. The latency for first mapping is much higher, about 30x of the rest. The rest re-populate and mapping latency is very low, less than 0.01 ms, and values are successfuuly carried. This sounds like a nice way for doing inter-process communication as we can pass the file scripter. 

```
./userfaultfd_hello_world
expected value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Modified page value: 97
Frist mapping latency: 2838996
Rest average latency: 94749.80
```

