# Advanced Systems Programming - Worksheet 2

## Contents
* [Overview](#overview)
* [Dependencies](#dependencies)
* [Compiling and Running the Files](#compiling-and-running-the-files)
* [Tasks](#tasks)
* [Observations](#observations)
* [Extra](#extra)

## Overview

This project provides efficient memory allocation using two different strategies: bump allocation using the bumping upwards and downwards methods. Additionally, it includes a benchmarking utility to measure the performance of functions utilising these memory allocation classes.

## Dependencies
* [Clang (5 or above)](https://clang.llvm.org/)
* [GNU Make](https://www.gnu.org/software/make/)

## Compiling and Running the Files

To compile the files, run the following (in the root directory of the worksheet) command in the terminal:

~~~bash
make
~~~

This will compile the files and generate the executable for each task accordingly. To run a specific executable, use the following command:

~~~bash
make run Task<task_number>
~~~

Example:

~~~bash
make run Task1
~~~

## Tasks
* [Task 1](#task-1)
* [Task 2](#task-2)
* [Task 3](#task-3)

### Overview

#### Bumping Up Allocation (bump/bump_up Class)

Efficient memory allocation using a simple bump allocation strategy.
It supports alignment requirements for allocated memory. 
Provides methods for allocation, deallocation, and printing the next address in the memory pool.

#### Bump Allocation (bump_down Class)

A variation of bump allocation with a different approach to handle alignment.
Efficient memory allocation with alignment support.
Includes methods for allocation, deallocation, and printing the next address in the memory pool.

#### Benchmarking Utility (benchmark Namespace)

A utility namespace for benchmarking functions using the memory allocation classes or any other function.
Measures the average time taken for a function over multiple runs.
Outputs benchmark results, including total runs and total time taken.


### Task 1

#### Bump Allocation Strategy:

The bump allocator follows a simple yet effective bump allocation strategy. This strategy involves allocating memory sequentially as requested, utilising a single pointer (`next`) which is is initialised to point to the beginning of the memory pool, to keep track of the next available memory location. It is incremenented after making sure that the alignment is correct and points to new address for further allocation accordingly.

Advantages of using a bump allocator for memory management rather than another allocator is that includes simplicity and efficiency, etc. Memory is allocated in a contiguous block, reducing fragmentation concerns (where the free memory space in a system becomes scattered or fragmented into small, non-contiguous chunks).

#### Alignment Support:

The allocator is designed to handle alignment requirements for allocated memory. Alignment here was important becuase it increases efficiency which is crucial for memory-related matters.

#### Alignment Logic: 
The allocator calculates the aligned address for each allocation by adjusting the raw address to the nearest higher multiple of the required alignment. This is achieved through bitwise operations.

~~~cpp
std::uintptr_t raw_address = reinterpret_cast<std::uintptr_t>(next);
std::uintptr_t mask = alignment - 1;
std::uintptr_t aligned_address = (raw_address + mask) & ~mask;
// Reference for logic used: https://stackoverflow.com/questions/227897/how-to-allocate-aligned-memory-only-using-the-standard-library
~~~

The `mask` is a binary pattern derived from the alignment requirement, used to adjust memory addresses. By adding this `mask` to the original `raw_address` and performing a bitwise AND operation with the complement of the `mask` (`~mask`), the resulting `aligned_address` is aligned according to the current allocation's requirements, ensuring that certain bits are set to zero and aligning the address to the next valid memory address based on the calculated alignment.

#### Reinterpret-casting pointers:

~~~cpp
if (reinterpret_cast<byte *>(aligned_address + bytes_needed) > pool + pool_size)
    return nullptr;

next = reinterpret_cast<byte *>(aligned_address + bytes_needed);
return reinterpret_cast<T *>(aligned_address);
~~~

Instead of using the C method of casting pointers to `void *` and back, the C++ method of `reinterpret_cast` is used. This is because `reinterpret_cast` is more explicit and safer than the C method, as it performs a compile-time check to ensure that the cast is valid. This is important as it ensures that the cast is valid at compile-time, rather than at run-time, which is the the case with the C method.

The `reinterpret_cast` is used to cast the `aligned_address + bytes_needed` to a `byte *` pointer, which is then used to check if the allocation is within the memory pool. If the allocation is valid, the `aligned_address` is cast to a `T *` (since it is the return type of the allocate function) and returned.

#### Output:

It can be seen in the output that the allocator allocates memory and that the alignment requirements for allocated memory correctly.

~~~bash
Before allocation: Address: 21700272 
After  allocating char:     Address: 21700273   (1 bytes - 7 padding)
After  allocating double:   Address: 21700288   (8 bytes - 0 padding)
After  allocating short:    Address: 21700290   (2 bytes - 0 padding)
After  allocating char:     Address: 21700291   (1 bytes - 1 padding)
After  allocating int:      Address: 21700296   (4 bytes - N/A padding)
~~~

*NOTE: Please note that the padding in parentheses () is not included in the actual output of the program. The padding is only included in thos output to show the alignment requirements for allocated memory and to aid in visualisation.*

### Task 2

The allocator for task 2 remained the same however unit tests were added to test the allocator. The unit tests were written using the [simpletest](https://github.com/kudaba/simpletest.git) framework.

The tests included:
* **A Full Allocator Test** Allocating more than the memory pool size and checking if the allocator returns a null pointer.
* **A Zero Sized Allocator Test** Initialising the allocator with a size of zero and checking if the throws an exception.
* **An Allocating Zero Test** Allocating zero bytes and checking if the allocator returns a null pointer.
* **An Oversized Allocator Test** Allocating the maximum number of bytes and checking if the allocator throws an exeption.
* **A Deallocate Test** Making sure that the deallocate function works as expected and allows for reallocation afterwards.
* **A Mixed Allocations And Deallocation Test** Allocating and deallocating memory in a of different sizes and types.
* **An Alignment And Multiple Data Types Test** Allocating memory with different alignment requirements and different data types and making sure memory is aligned.
* **A Massive Allocation Test** Initialising the allocator with 4GB of memory and allocating 4GB of memory and making sure that the allocator does not return a nullptr.
* **A Struct Allocation Test** Allocating a struct and making sure that the allocator can handle struct data types and does not return a nullptr.
* **A Union Allocation Test** Allocating a union and making sure that the allocator can handle union data types and does not return a nullptr.
* **A Memory Leak Detection Test** Creating an allocator in a scope and making sure that the allocator is destroyed after the scope ends. Output:

~~~bash
// NO DEALLOCATION INCLUDED 
(Scoped Bump) Address (before allocation): Address: 23516576
(Scoped Bump) Address (after allocation of 10 ints): Address: 23516616
Address (after destruction): Address: 23516576
Address (after allocation of 10 ints): Address: 23516616
~~~
* **A Repeated Allocator Destruction And Creation Test:** Creating and destroying an allocator multiple times using a loop and allocating memory, making sure that there is no memory leak or any other issues.

#### Output:

~~~
Running all tests in groups [Bump].
Running [Bump/FullAllocatorTest]: Passed 3 out of 3 tests in 3e-06 seconds
Running [Bump/ZeroSizedAllocatorTest]: Passed 1 out of 1 tests in 3.5e-05 seconds
Running [Bump/AllocatingZeroTest]: Passed 1 out of 1 tests in 1e-06 seconds
Running [Bump/OversizedAllocatorTest]: Passed 1 out of 1 tests in 6e-06 seconds
Running [Bump/DeallocateTest]: Passed 2 out of 2 tests in 0 seconds
Running [Bump/MixedAllocationsAndDeallocationTest]: Passed 4 out of 4 tests in 1e-06 seconds
Running [Bump/AlignmentAndMultipleDataTypesTest]: Passed 8 out of 8 tests in 1e-06 seconds
Running [Bump/MassiveAllocationTest]: Passed 1 out of 1 tests in 1.9e-05 seconds
Running [Bump/StructAllocationTest]: Passed 3 out of 3 tests in 1e-06 seconds
Running [Bump/UnionAllocationTest]: Passed 3 out of 3 tests in 1e-06 seconds
Running [Bump/MemoryLeakDetectionTest]
Address (before allocation): Address: 31471008
Address (after allocation of 10 ints): Address: 31471048
Address (after destruction): Address: 31471008
Address (after allocation of 10 ints): Address: 31471048
: Passed 2 out of 2 tests in 3.4e-05 seconds
Running [Bump/RepeatedAllocatorDestructionAndCreationTest]: Passed 5 out of 5 tests in 1e-06 seconds
12 Tests finished. All 34 assertions are passing.
~~~

### Task 3

#### Bump Down Allocation Strategy:

The bump down allocator is a variation of the bump up allocator with a different approach to handle alignment. The bump down allocator allocates memory in the opposite direction of the bump up allocator, starting from the end of the memory pool and moving downwards. This is achieved by using a pointer (`next`) which is initialised to point to the end of the memory pool, and is decremented after making sure that the alignment is correct and points to new address for further allocation accordingly. 

The bump_down allocator is considered potentially more efficient than the bump up allocator due to its alignment. By aligning memory blocks downward, it aims to reduce internal fragmentation, minimising wasted space within allocated blocks. This alignment strategy may offer benefits in scenarios where objects of varying sizes are frequently allocated and deallocated. Additionally, aligning downwards simplifies the deallocation process and might result in faster pointer comparisons during allocation.


The entire allocator remained the same however the following changes were made in the allocate function:

~~~cpp
std::size_t bytes_needed = sizeof(T) * n;
std::size_t alignment = alignof(T);
std::size_t space_needed = bytes_needed + alignment - 1;

std::uintptr_t raw_address = reinterpret_cast<std::uintptr_t>(next);
std::uintptr_t mask = ~(alignment - 1);
std::uintptr_t aligned_address = (raw_address - bytes_needed) & mask;

if (reinterpret_cast<byte *>(aligned_address) < pool)
    return nullptr;

next = reinterpret_cast<byte *>(aligned_address);
return reinterpret_cast<T *>(aligned_address);
~~~

#### Comparison of Bump Up and Bump Down Allocators

| Aspect | `bump_down` Allocator | `bump_up` Allocator | Explanation |
| ------ | --------------------- | ------------------- | ----------- |
| **Alignment Adjustment** | Adjusts raw address downward | Adjusts raw address upward | `bump_down` aligns memory downward to potentially reduce internal fragmentation. Adjusting the raw address downward ensures that the allocated blocks maintain alignment based on the pool's original alignment, aiding efficient memory utilisation.<br> `bump_up` aligns memory upward. Adjusting the raw address upward ensures that the allocated blocks maintain alignment based on the pool's original alignment. |
| **Allocation Check** | `if (aligned_address < pool)` | `if (aligned_address + bytes_needed > pool + pool_size)` | `bump_down` checks if the aligned address is less than the pool's start. If true, it means the allocation exceeds the memory pool's boundaries, and `nullptr` is returned. This check ensures that the allocated memory is within the bounds of the pool and avoids potential overflows.<br> `bump_up` checks if the end address of the allocation `(aligned_address + bytes_needed)` is greater than the pool's end. If true, it means the allocation exceeds the memory pool's boundaries, and `nullptr` is returned. This condition ensures that the allocated memory, including its size, fits within the memory pool. |
| **Alignment Calculation** | `std::uintptr_t mask = ~(alignment - 1);`<br>`std::uintptr_t aligned_address = (raw_address - bytes_needed) & mask;` | `std::uintptr_t mask = alignment - 1;`<br>`std::uintptr_t aligned_address = (raw_address + mask) & ~mask;` | `bump_down` calculates the aligned address by adjusting the raw address to the nearest lower multiple of the required alignment. This is achieved by subtracting the bytes needed from the raw address and performing a bitwise AND operation with the complement of the `mask`. This ensures that certain bits are set to zero and aligns the address to the next valid memory address based on the calculated alignment.<br> `bump_up` calculates the aligned address by adjusting the raw address to the nearest higher multiple of the required alignment. This is achieved by adding the `mask` to the raw address and performing a bitwise AND operation with the complement of the `mask`. This ensures that certain bits are set to zero and aligns the address to the next valid memory address based on the calculated alignment. |

#### Benchmarking Utility

The benchmarking utility is a namespace that provides a variadic templated function to measure the performance of functions using the memory allocation classes or any other function. It takes a function with any number of arguments thanks to the variadic template. It measures and return the average time taken for a function over a number of runs provided by the user and outputs benchmark results, including total runs and total time taken.

The benchmarking utility uses the `std::chrono` library to measure the time taken for a function to execute. The `std::chrono` library provides a set of classes for time-related functions, including clocks, time points, and durations. 
* The `std::chrono::high_resolution_clock` is used to measure the time taken for a function to execute. This clock is the most precise clock available on the system, and is used to measure the time taken for a function to execute. A macro was used as an alias for this clock to simplify the code. 
* The `std::chrono::time_point` class is used to represent a point in time, and is used to store the start and end times of the function. A typedef was used as an alias for this class to simplify the code. 
* The `std::chrono::duration` class is used to represent a time interval, and is used to calculate the time taken for the function to execute. A macro was used as an alias for this class to simplify the code.

The main part of the code used that executes the function and measures the time taken is:

~~~cpp
for (std::size_t i = 0; i < runs; ++i)
{
    time_point start = time_now();
    func(std::forward<Args>(args)...);
    time_point end = time_now();

    total_time += duration(end - start);
}
~~~

The `for` loop executes the function `runs` number of times, and measures the time taken for each run. The `time_now()` function is used to get the current time, which is stored in the `start` variable. The function is then executed, and the current time is stored in the `end` variable. The time taken for the function to execute is calculated by subtracting the `start` time from the `end` time, and is added to the `total_time` variable. This is repeated for the number of runs specified by the user.

The `std::forward` function is used to forward the arguments to the function. This is used to ensure that the arguments are forwarded as either lvalues or rvalues, depending on the type of the argument. This is important as it ensures that the arguments are forwarded correctly, and avoids unnecessary copying of arguments.

<!-- An r-value reference differs from an l-value reference in that it can bind to a temporary object (an r-value), whereas an l-value reference can only bind to an l-value. -->
The tests included (for bump_up and bump_down allocators):
* **A Single Allocation Test** Allocating a single integer by using a lambda function and passing the parameter as an r-value reference. *
* **A Big Allocation Test** Allocating a large amount of memory by using a lambda function and passing the parameter as an r-value reference. *
* **A Mixed Size Allocation Test** Allocating memory of different sizes and types by using a void function that initialises the bumper in the body of the function.
* **A Loop Allocation And Deallocation Test** Allocating and deallocating memory in a loop by using a function that takes an l-value reference of the bumper object and another that uses r-value reference. *

\* An r-value reference is when a reference is bound to an r-value. An r-value is a temporary object that is created and destroyed during the same expression. An l-value reference is when a reference is bound to an l-value. An l-value is an object that persists beyond a single expression. Reflection on the results of the tests is provided in the [Observations](#observations) section.

#### Output:

~~~
Benching: Single Allocations (Bump Up (lambda: pass by r-value reference (&&)))
Total runs: 100
Total time taken: 3500ns
Average time taken per run: 35ns

Benching: Single Allocations (Bump Down (lambda: pass by r-value reference (&&)))
Total runs: 100
Total time taken: 3201ns
Average time taken per run: 32.01ns

Benching: Big Allocations (Bump Up (lambda: pass by r-value reference (&&)))
Total runs: 100
Total time taken: 5300ns
Average time taken per run: 53ns

Benching: Big Allocations (Bump Down (lambda: pass by r-value reference (&&)))
Total runs: 100
Total time taken: 5100ns
Average time taken per run: 51ns

Benching: Mixed Size Allocations (Bump Up (void function(void)))
Total runs: 100
Total time taken: 11100ns
Average time taken per run: 111ns

Benching: Mixed Size Allocations (Bump Down (void function(void)))
Total runs: 100
Total time taken: 9501ns
Average time taken per run: 95.01ns

Benching: Loop Allocations and Deallocation (Bump Up (void function(pass by l-value ref (&))))
Total runs: 1
Total time taken: 900ns
Average time taken per run: 900ns

Benching: Loop Allocations and Deallocation (Bump Up (lambda: pass by rvr))
Total runs: 1
Total time taken: 800ns
Average time taken per run: 800ns

Benching: Loop Allocations and Deallocation (Bump Down (void function(pass by l-value ref (&))))
Total runs: 1
Total time taken: 801ns
Average time taken per run: 801ns

Benching: Loop Allocations and Deallocation (Bump Down (lambda: pass by rvr))
Total runs: 1
Total time taken: 700ns
Average time taken per run: 700ns
~~~

## Observations

1. During testing, it was observed that initialising the allocator with a size of 4 and then allocating 1 integer caused a segmentation fault in a function of format void function(void). However this is not the case when using l/r-value referencing.

2. In regular use cases of the bump allocator, bumping downwards is almost always efficient. However during some tests the bump_up would take less time to accomplish the task given. This could be because compiler may optimise code differently, and the generated assembly might favor one allocator over the other. The specifics of how the compiler optimizes the code can influence performance of either allocator.

3. Passing void to a function and initialising the bumper object in body is approximately 3-4 times slower than passing by r-value according to multiple runs of this, however bump_down is faster than bump_up in this case.

4. When the allocator is passes by l-value reference to a function, the object is being reused (same allocator) for all iterations of the benchmark. This means that each allocation is made in a memory pool that's already partially filled from the previous iterations, which could lead to fragmentation and slower allocations.
On the other hand, when the allocator is passed by r-value reference to lambda, a new allocator is being created for each iteration of the benchmark. This means that each allocation is made in a fresh memory pool, which may have potentially been the cause of r-value being faster than l-value.

## Extra

### Benchmarking with different compilers and levels of optimisation

#### clang++ vs g++ (No optimisation (O0))

| Benchmark | Bump Up (g++) | Bump Down (g++) | Bump Up (clang++) | Bump Down (clang++) |
|------------------------------------------------|------------------------------|------------------------------|------------------------------|------------------------------|
| **Single Allocations** | | | | | | 
| Total runs | 100 | 100 | 100 | 100 | 
| Total time taken | 3900ns | 3201ns | 4200ns | 3800ns |
| Average time per run | 39ns | 32.01ns | 42ns | 38ns |
| | | | | |
| **Big Allocations (pass by r-value ref)** | | | | |
| Total runs | 100 | 100 | 100 | 100 |
| Total time taken | 5200ns | 5100ns | 4700ns | 5000ns |
| Average time per run | 52ns | 51ns | 47ns | 50ns |
| | | | | |
| **Mixed Size Allocations (void function(void))** | | | | |
| Total runs | 100 | 100 | 100 | 100 |
| Total time taken | 10200ns | 8501ns | 10900ns | 9300ns |
| Average time per run | 102ns | 85.01ns | 109ns | 93ns |
| | | | | |
| **Loop Alloc and Dealloc (λ: pass by l-value ref (&))** | | | | |
| Total runs        | 1   | 1   | 1   | 1   |
| Total time taken   | 700ns  | 700ns  | 800ns  | 800ns  |
| Average time per run   | 700ns  | 700ns  | 800ns  | 800ns  |
| | | | | |
| **Loop Alloc and Dealloc (λ: pass by r-value ref)** | | | | |
| Total runs | 1 | 1 | 1 | 1 |
| Total time taken | 600ns | 700ns | 700ns | 700ns |
| Average time per run | 600ns | 700ns | 700ns | 700ns |

#### clang++ vs g++ (Aggressive optimisation (O3)) 

| Benchmark | Bump Up (g++) | Bump Down (g++) | Bump Up (clang++) | Bump Down (clang++) |
|------------------------------------------------|------------------------------|------------------------------|------------------------------|------------------------------|
| **Single Allocations (pass by r-value ref)** | | | | |
| Total runs | 100 | 100 | 100 | 100 |
| Total time taken   | 2301ns | 1800ns | 2200ns | 2600ns |
| Average time per run   | 23.01ns     | 18ns   | 22ns   | 26ns   |
|   | | | | |
| **Big Allocations (λ: pass by r-value ref)**    | | | | |
| Total runs   | 100 | 100 | 100 | 100 |
| Total time taken   | 3000ns | 3100ns | 2400ns | 2300ns |
| Average time per run   | 30ns   | 31ns   | 24ns   | 23ns   |
|   | | | | |
| **Mixed Size Allocations (void function(void))** | | | | |
| Total runs   | 100 | 100 | 100 | 100 |
| Total time taken   | 7500ns | 5000ns | 7100ns | 5501ns |
| Average time per run   | 75ns   | 50ns   | 71ns   | 55.01ns     |
|   | | | | |
| **Loop Alloc and Dealloc (λ: pass by l-value ref (&))** |     | | | |
| Total runs   | 1   | 1   | 1   | 1   |
| Total time taken   | 100ns  | 0ns | 300ns  | 0ns |
| Average time per run   | 100ns  | 0ns | 300ns  | 0ns |
|   | | | | |
| **Loop Alloc and Dealloc (λ: pass by r-value ref)** | | | | |
| Total runs   | 1   | 1   | 1   | 1   |
| Total time taken   | 0ns | 100ns  | 200ns  | 200ns  |
| Average time per run   | 0ns | 100ns  | 200ns  | 200ns  |

# [Back To Top](#advanced-systems-programming---worksheet-2)