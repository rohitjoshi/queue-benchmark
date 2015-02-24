# queue-benchmark
Here are comparison between boost lock free queues and moodycamel queues.

1. Multi Consumers and Multi Producers:   boost::lockfree::queue , boost::lockfree::stack, and moodycamel::ConcurrentQueue for concurrent/ access.
2. Single Consumer and Single Producer: boost::lockfree::spsc_queue and moodycamel::ReaderWriterQueue.  

Observations:

1. For single consumer/single producer: moodycamel::ReaderWriterQueue  is faster than boost::lockfree::spsc_queue

2. For concurrent access, moodycamel::ConcurrentQueue outperforms both boost queue and stack

3. For some reason, moodycamel::ConcurrentQueue has better performance when 3 or 4 producers/consumers are used than 1 or 2 threads.  


boost::lockfree::queue:

    boost::lockfree::queue: Producer threads:1, Consumer threads:1, producer_counter: 10000000, consumer_counter 10000000. Time      Taken:2.52799 seconds.
    boost::lockfree::queue: Producer threads:2, Consumer threads:2, producer_counter: 10000000, consumer_counter 10000000. Time      Taken:2.63741 seconds.
    boost::lockfree::queue: Producer threads:3, Consumer threads:3, producer_counter: 9999999, consumer_counter 9999999. Time        Taken:2.74344 seconds.
    boost::lockfree::queue: Producer threads:4, Consumer threads:4, producer_counter: 10000000, consumer_counter 10000000. Time      Taken:2.93172 seconds.

boost::lockfree::stack:

    boost::lockfree::stack: Producer threads:1, Consumer threads:1, producer_counter: 10000000, consumer_counter 10000000. Time      Taken:2.22722 seconds.
    boost::lockfree::stack: Producer threads:2, Consumer threads:2, producer_counter: 10000000, consumer_counter 10000000. Time      Taken:2.58529 seconds.
    boost::lockfree::stack: Producer threads:3, Consumer threads:3, producer_counter: 9999999, consumer_counter 9999999. Time        Taken:2.81599 seconds.
    boost::lockfree::stack: Producer threads:4, Consumer threads:4, producer_counter: 10000000, consumer_counter 10000000. Time      Taken:3.22939 seconds.



moodycamel::ConcurrentQueue:

    moodycamel::ConcurrentQueue: Producer threads:1, Consumer threads:1, producer_counter: 10000000, consumer_counter 10000000.      Time Taken:1.49435 seconds.
    moodycamel::ConcurrentQueue: Producer threads:2, Consumer threads:2, producer_counter: 10000000, consumer_counter 10000000.      Time Taken:1.31297 seconds.
    moodycamel::ConcurrentQueue: Producer threads:3, Consumer threads:3, producer_counter: 9999999, consumer_counter 9999999.        Time Taken:0.868555 seconds.
    moodycamel::ConcurrentQueue: Producer threads:4, Consumer threads:4, producer_counter: 10000000, consumer_counter 10000000.      Time Taken:0.810693 seconds.

Single Producer / Single Consumer:

boost::lockfree::spsc_queue:

    boost::lockfree::spsc_queue: Producer threads:1, Consumer threads:1, producer_counter: 10000000, consumer_counter 10000000. Time Taken:0.776101 seconds.

moodycamel::ReaderWriterQueue:

    moodycamel::ReaderWriterQueue: Producer threads:1, Consumer threads:1, producer_counter: 10000000, consumer_counter 10000000. Time Taken:0.586292 seconds.
