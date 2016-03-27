/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: gunrocky
 * Task: Exploring the differences and purposes of various synchronization primitives
 * in practice Write a program that can run N parallel threads (N may change during startup).
 * Threads only in large loops increment a single shared variable
 * and perform minimal latency (1 millisecond to simulate busy computing activity).
 * Provide control over the blocking mode of the thread's working cycle, at a minimum:
 * a) no blocking at all,
 * b) locking on a mutex,
 * c) lock on spinlock.
 * Analyze the difference in the behavior of the 3 options.
 *
 * Created on March 27, 2016, 12:34 PM
 */

#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/syscall.h>
#include <string.h>
#include <vector>
#include <memory>
#include <sys/time.h>


// condition for threads to start
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_cond_mutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef USE_MUTEX
// control mutex
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
#elif USE_SPINLOCK
// control spinlock
pthread_spinlock_t g_spinlock;
#endif

// what to count
static unsigned long long g_val = 0;

// when to stop count
static const unsigned long long g_count = 250000;

// quantity of ready threads
static sig_atomic_t g_threads_ready = 0;

// thread info structure

struct thread_info {
    pthread_t thread_id;
    int thread_num;
};

void * thread_function(void *arg) {

    int *pnum = reinterpret_cast<int *> (arg);

#ifdef USE_MUTEX
    pthread_mutex_lock(&g_mutex);
#elif USE_SPINLOCK
    pthread_spin_lock(&g_spinlock);
#endif

    int num = *pnum;
    std::cout << "thread " << num << std::endl;
    ++g_threads_ready;

#ifdef USE_MUTEX
    pthread_mutex_unlock(&g_mutex);
#elif USE_SPINLOCK
    pthread_spin_unlock(&g_spinlock);
#endif

    // all threads wait to start
    pthread_mutex_lock(&g_cond_mutex);
    pthread_cond_wait(&g_cond, &g_cond_mutex);
    pthread_mutex_unlock(&g_cond_mutex);

    while (g_val < g_count) {

#ifdef USE_MUTEX
        pthread_mutex_lock(&g_mutex);
#elif USE_SPINLOCK
        pthread_spin_lock(&g_spinlock);
#endif

        // count variable;
        ++g_val;

#ifdef USE_MUTEX
        pthread_mutex_unlock(&g_mutex);
#elif USE_SPINLOCK
        pthread_spin_unlock(&g_spinlock);
#endif

        //fflush(stdout);
        usleep(1000);
    }
    return (void *) 0;
}

int main(int argc, char** argv) {

    if (argc < 2 || argc > 2) {
        std::cerr << "usage: " << argv[0] << " <number of threads>\n";
        return 1;
    }

    for (size_t i = 0; i < strlen(argv[1]); ++i) {
        if (!isdigit(argv[1][i])) {
            std::cerr << argv[1][i] << " is not digit\n";
            return 1;
        }
    }

    size_t num_threads = static_cast<size_t>(atoi(argv[1]));
    if (num_threads < 1) {
        std::cerr << "input parameter must be positive\n";
        return 1;
    }
#ifdef USE_SPINLOCK
    pthread_spin_init(&g_spinlock, PTHREAD_PROCESS_PRIVATE);
#endif

    std::cout << "Type of sync: ";
#ifdef USE_MUTEX
    std::cout << "mutex";
#elif USE_SPINLOCK
    std::cout << "spinlock";
#else
    std::cout << "no sync";
#endif
    std::cout << "\nQuantity of threads - " << num_threads << std::endl;
    std::cout << "Top value - " << g_count << std::endl;

    std::vector<struct thread_info> thrs;
    int error = 0;
    for (size_t i = 0; i < num_threads; ++i) {
        thrs.push_back(thread_info());
        thrs[i].thread_num = static_cast<int>(i);
        if ((error = pthread_create(&thrs[i].thread_id, NULL, thread_function, &thrs[i].thread_num))) {
            std::cerr << "thread " << i << "; pthread_create() err = " << error << std::endl;
            thrs.pop_back();
        }
    }

    // wait until all threads will be ready
    while (true) {
#ifdef USE_MUTEX
        pthread_mutex_lock(&g_mutex);
#elif USE_SPINLOCK
        pthread_spin_lock(&g_spinlock);
#else
        std::cout << "just sleep to give time for threads\n";
        sleep(10);
        break;
#endif

        if (g_threads_ready == static_cast<int>(thrs.size())) {
#ifdef USE_MUTEX
            pthread_mutex_unlock(&g_mutex);
#elif USE_SPINLOCK
            pthread_spin_unlock(&g_spinlock);
#endif
            break;
        }

#ifdef USE_MUTEX
        pthread_mutex_unlock(&g_mutex);
#elif USE_SPINLOCK
        pthread_spin_unlock(&g_spinlock);
#endif
    }
    struct timeval tv1 {}, tv2 {};
    gettimeofday(&tv1, NULL);

    // make all threads start
    pthread_mutex_lock(&g_cond_mutex);
    pthread_cond_broadcast(&g_cond);
    pthread_mutex_unlock(&g_cond_mutex);

    // wait for all treads
    for (std::vector<struct thread_info>::iterator it = thrs.begin(); it != thrs.end(); ++it) {
        if ((error = pthread_join(it->thread_id, NULL))) {
            std::cerr << "thread " << it->thread_num << "; pthread_join() err = " << error << std::endl;
        }
        // it needs because output hangs without it
        //fflush(stdout);
    }
#ifdef USE_SPINLOCK
    pthread_spin_destroy(&g_spinlock);
#endif

    gettimeofday(&tv2, NULL);

    if (tv1.tv_usec > tv2.tv_usec) {
        tv2.tv_sec--;
        tv2.tv_usec += 1000000;
    }

    std::cout << "Result - " << tv2.tv_sec - tv1.tv_sec << "." << tv2.tv_usec - tv1.tv_usec << std::endl;
    std::cout << "Counted value - " << g_val << std::endl;

    return 0;
}

