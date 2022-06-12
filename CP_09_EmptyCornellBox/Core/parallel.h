//
// Created by star on 2/16/2022.
//

#ifndef ONE_WEEKEND_PARALLEL_H
#define ONE_WEEKEND_PARALLEL_H

#include <iostream>
#include <thread>
#include <future>
#include <execution>


template<typename Iter, typename F>
void parallel_for_each_async_pool(Iter begin, Iter end, F &&f,int threadNum= std::thread::hardware_concurrency()){
    auto whole_block_size = std::distance(begin,end);
    std::cout << "parallel block size:" << whole_block_size << std::endl;
    if(whole_block_size < 2)
    {
        std::for_each(begin, end, std::forward<F>(f));
        return ;
    }
    auto num_threads = threadNum;
    if(num_threads > whole_block_size) {
        std::for_each(std::execution::par, begin, end, std::forward<F>(f));
        return;
    }

    int part = whole_block_size / num_threads;
    std::cout << "per block size:" << part << std::endl;

    auto block_begin = begin;
    auto block_end = block_begin;

    std::vector<std::future<void> > threads;
    threads.reserve(num_threads);
    for(unsigned i=0;i<num_threads; i++){

        if(i == num_threads-1)
            block_end = end;
        else
            std::advance(block_end ,part);

        threads.emplace_back(std::async([=, &f](){//  注意一定要将block_begin , block_end 按值传递，要不std::transform应该会在里面std::advance()这遍历器
            std::for_each(block_begin, block_end, std::forward<F>(f));
        })) ;
        block_begin = block_end;
    }
    for(auto &t : threads)
        t.wait();
}


#endif //ONE_WEEKEND_PARALLEL_H
