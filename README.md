# diff_sync

Exploring the differences and purposes of various synchronization primitives in practice.

Write a program that can run N parallel threads (N may change during startup).
Threads only in large loops increment a single shared variable and perform minimal latency (1 millisecond to simulate busy computing activity).
Provide control over the blocking mode of the thread's working cycle, at a minimum:

* no blocking at all,
* locking on a mutex,
* lock on spinlock.

Analyze the difference in the behavior of the 3 options.
