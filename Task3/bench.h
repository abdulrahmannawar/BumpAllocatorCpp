#pragma once

#include <iostream>
#include <chrono>
#include <utility>
#include <functional>

#define time_now() std::chrono::high_resolution_clock::now();
#define duration(x) std::chrono::duration_cast<std::chrono::nanoseconds>(x).count();

namespace benchmark
{
    typedef std::chrono::high_resolution_clock::time_point time_point;

    template <typename Function, typename... Args>
    double run_benchmark(const std::string &description, std::size_t runs, Function func, Args &&...args)
    {
        double total_time = 0.0;

        for (std::size_t i = 0; i < runs; ++i)
        {
            time_point start = time_now();
            func(std::forward<Args>(args)...);
            time_point end = time_now();
            total_time += duration(end - start);
        }

        std::cout << "Benching: " << description << std::endl;
        std::cout << "Total runs: " << runs << std::endl;
        std::cout << "Total time taken: " << total_time << "μs" << std::endl;
        double average_time = total_time / static_cast<double>(runs);
        return average_time;
    }
}