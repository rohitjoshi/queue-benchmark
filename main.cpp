/* 
 * File:   main.cpp
 * Author: ytr289
 *
 * Created on February 10, 2015, 1:35 PM
 */

#include <cstdlib>
#include <thread>
#include <iostream>
//#include <atomic>
#include <boost/atomic.hpp>
//#include "boost/interprocess/detail/atomic.hpp"
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/stack.hpp>
#include "concurrentqueue.h"
using namespace std;
using namespace moodycamel;
using namespace std::chrono;

/*
 * 
 */

template<typename T, typename D>
void producer(T &queue, const D &d, boost::atomic_int &counter, unsigned iterations, bool use_counter)
{
    
    for (int i = 0; i != iterations; ++i) {
       // int value = i;
        if(queue.push(d) && use_counter) {
           ++counter;
        }
    }
    
}

template<>
void producer<moodycamel::ConcurrentQueue<int>, int>(moodycamel::ConcurrentQueue<int> &queue, const int &d, boost::atomic_int &counter, unsigned iterations, bool use_counter)
{
    moodycamel::ProducerToken ptok(queue);
    for (int i = 0; i != iterations; ++i) {
      //  int value = i;
        
        if(queue.enqueue(ptok, d) && use_counter) {
           ++counter;
        }
    }
    
}
template<>
void producer<moodycamel::ConcurrentQueue<std::string>, std::string>(moodycamel::ConcurrentQueue<std::string> &queue, const std::string &data, boost::atomic_int &counter, unsigned iterations, bool use_counter)
{
    moodycamel::ProducerToken ptok(queue);
    for (int i = 0; i != iterations; ++i) {
      
        if(queue.enqueue(data) && use_counter) {
           ++counter;
        }
    }
    
}

template<typename T, typename D>
void consumer(T &queue,  boost::atomic<bool> &done, boost::atomic_int &counter, bool use_counter)
{
    D value;
    
    while (!done) {
        while (queue.pop(value)) {
            if (use_counter)++counter;
        }
    }

    while (queue.pop(value)) {
         if (use_counter)++counter;
    }
    
}

template<>
void consumer<moodycamel::ConcurrentQueue<std::string>, std::string > (moodycamel::ConcurrentQueue<std::string> &queue,  boost::atomic<bool> &done, boost::atomic_int &counter, bool use_counter)
{
    
    std::string value;
    moodycamel::ConsumerToken ctok(queue);
    while (!done) {
        while (queue.try_dequeue(ctok, value)) {
            if (use_counter)++counter;
        }
    }

    while (queue.try_dequeue(ctok, value)) {
         if (use_counter)++counter;
    }
    
}
template<>
void consumer<moodycamel::ConcurrentQueue<int>, int > (moodycamel::ConcurrentQueue<int> &queue,  boost::atomic<bool> &done, boost::atomic_int &counter, bool use_counter)
{
    
    int value;
    moodycamel::ConsumerToken ctok(queue);
    while (!done) {
        while (queue.try_dequeue(ctok, value)) {
            if (use_counter)++counter;
        }
    }

    while (queue.try_dequeue(ctok, value)) {
         if (use_counter)++counter;
    }
    
}

template<typename T, typename D>
void test_boost_lockfree(const std::string & queue_type, T &queue, const D &d, unsigned iterations,  bool use_counters, unsigned producer_threads, unsigned consumer_threads) {
    std::thread producers_group[producer_threads];
    std::thread consumers_group[consumer_threads];
    
    boost::atomic<bool> done(false);
    boost::atomic_int producer_counter(0);
    boost::atomic_int consumer_counter(0);
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    for (unsigned i = 0; i != producer_threads; ++i) {
      
        producers_group[i] = std::thread([&]() {
            producer< T, D >(queue, d, producer_counter, iterations/producer_threads,use_counters);
        });
    }
    
    for (unsigned i = 0; i != consumer_threads; ++i) {
      
        consumers_group[i] = std::thread([&]() {
            consumer< T, D >( queue, done, consumer_counter,use_counters );
        });
    }
    
    for (int i = 0; i != producer_threads; ++i) {
      
        producers_group[i].join();
    }
    done = true;
    
    for (int i = 0; i != consumer_threads; ++i) {
      
        consumers_group[i].join();
    }
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    
    std::cout << queue_type << ": Producer threads:" << producer_threads << ", Consumer threads:" << consumer_threads << ", producer_counter: " << producer_counter << ", consumer_counter " << consumer_counter << ". Time Taken:" << time_span.count() << " seconds.\n";
    
}

void test_int() {
    std::cout << "\n\nTesting size of integer" << std::endl;
    unsigned init_size = 1024*1024;
    {
        for(unsigned i = 0; i < 4; i++) {
            boost::lockfree::queue<int> queue(init_size);
            test_boost_lockfree<boost::lockfree::queue<int>, int >("boost::lockfree::queue", queue, 1, 10000000, true, i+1,i+1);
        }
    }
    {
        for(unsigned i = 0; i < 4; i++) {
            boost::lockfree::stack<int> queue(init_size);
            test_boost_lockfree<boost::lockfree::stack<int>, int >("boost::lockfree::stack", queue, 1, 10000000, true, i+1,i+1);
        }
    }
    {
            boost::lockfree::spsc_queue<int> queue(init_size*1024);
            test_boost_lockfree<boost::lockfree::spsc_queue<int>, int >("boost::lockfree::spsc_queue", queue, 1, 10000000, true, 1,1);       
    }
    
    {
        for(unsigned i = 0; i < 4; i++) {
            moodycamel::ConcurrentQueue<int> queue(init_size*1024);
            test_boost_lockfree<moodycamel::ConcurrentQueue<int>, int >("moodycamel::ConcurrentQueue", queue, 1, 10000000, true, i+1,i+1);  
        }
    }
    
}
void test_100() {
    std::cout << "Testing 100 bytes" << std::endl;
     unsigned init_size = 1024*1024;
    const std::string data = "With a function template, you can define special behavior for a specific type by providing an explic";
    
    
    {
        for(unsigned i = 0; i < 4; i++) {
            moodycamel::ConcurrentQueue<std::string> queue(init_size);
            test_boost_lockfree<moodycamel::ConcurrentQueue<std::string>, std::string >("moodycamel::ConcurrentQueue", queue, data, 10000000, true, i+1,i+1);  
        }
    }
}
int main(int argc, char* argv[])
{
    test_int();
   test_100();
    return 0;
}


