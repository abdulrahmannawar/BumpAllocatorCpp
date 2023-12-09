#pragma once

#include <iostream>
#include <chrono>
#include <utility>
#include <functional>

// Macro to get the current time
#define time_now() std::chrono::high_resolution_clock::now();

// Macro to calculate the duration in nanoseconds
#define duration(x) std::chrono::duration_cast<std::chrono::nanoseconds>(x).count();


namespace benchmark
{
    // Alias for a time point using high-resolution clock
    typedef std::chrono::high_resolution_clock::time_point time_point;

    // Function to run a benchmark for a given function
    template <typename Function, typename... Args>
    double run_benchmark(const std::string &description, std::size_t runs, Function func, Args &&...args)
    {
        double total_time = 0.0;

        // Loop for the specified number of runs
        for (std::size_t i = 0; i < runs; ++i)
        {
            // Record the start time
            time_point start = time_now();

            // Call the provided function using std::forward
            func(std::forward<Args>(args)...);

            // Record the end time
            time_point end = time_now();

            // Calculate the duration and add it to the total time
            total_time += duration(end - start);
        }

        // Output benchmark results
        std::cout << "Benching: " << description << std::endl;
        std::cout << "Total runs: " << runs << std::endl;
        std::cout << "Total time taken: " << total_time << "μs" << std::endl;

        // Calculate and output the average time per run
        double average_time = total_time / static_cast<double>(runs);
        return average_time;
    }
} // namespace benchmark
