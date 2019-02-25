/* 
 * File:   main.cpp
 * Author: Rohit joshi
 *
 * Created on February 24, 2015, 10:43 AM
 */

#include <cstdlib>
#include "queue_test.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    run_test();
    test_1KQueueSize();
    test_1MB_QueueSize();
    test_Default_QueueSize();
    return 0;
}

