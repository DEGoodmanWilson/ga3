//
// Created by Donald Goodman-Wilson on 18/09/2018.
//

#include <catch.hpp>
#include "test.h"
#include <ThreadPool/ThreadPool.hpp>


SCENARIO("ThreadPool")
{
    GIVEN("The default thread pool")
    {
        THEN("It should use 7 threads to process")
        {
            const auto start = std::chrono::high_resolution_clock::now();
            std::vector<ThreadPool::ThreadPool::TaskFuture<void>> v;

            static std::atomic_uint16_t threads{0}, peak_threads{0};
            static std::atomic_uint64_t count{0};


            for (std::uint32_t i = 0u; i < 21u; ++i)
            {
                v.push_back(ThreadPool::DefaultThreadPool::submitJob([]()
                                                                     {
                                                                         count++;
                                                                         threads++;
                                                                         if (threads > peak_threads)
                                                                         {
                                                                             uint8_t temp = threads;
                                                                             peak_threads = temp;
                                                                         }

                                                                         std::this_thread::sleep_for(std::chrono::milliseconds(100));

                                                                         threads--;
                                                                     }));
            }
            for (auto &item: v)
            {
                item.get();
            }
            const auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;

            REQUIRE(diff.count() <= 0.1*8); // 8 for wiggle room
            REQUIRE(peak_threads >= 7);
        }
    }
}