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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    return 0;
}

