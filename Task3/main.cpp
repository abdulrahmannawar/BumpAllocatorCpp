#include "bump.h"
#include "bench.h"
#include <iostream>

// obs initialsing allocator to 4 and then allocating 1 int caused seg fault
// obs in single allocation: Compilers can optimize code differently, and the generated assembly might favor one allocator over the other. The specifics of how the compiler optimizes your code can influence performance.
// obs: passing by r-value is mostly faster or sometimes similar in bump_up than bump_down
// obs: passing void to function and init object in body is approximately 3-4 times slower than passing by r-value however bump_down is faster than bump_up in this case
/* obs:
When you pass the allocator by l-value reference, you're reusing the same allocator for all iterations of the benchmark. This means that each allocation is made in a memory pool that's already partially filled from the previous iterations, which could lead to fragmentation and slower allocations.
On the other hand, when you pass the allocator by r-value reference, you're creating a new allocator for each iteration of the benchmark. This means that each allocation is made in a fresh memory pool, which could lead to faster allocations.
*/

void MixedSizeAllocationsBumpUp(void);
void MixedSizeAllocationsBumpDown(void);
void LoopAllocationAndDeallocationBumpUp(bump::bump_up<4096> &);
void LoopAllocationAndDeallocationBumpDown(bump::bump_down<4096> &);

void test(bump::bump_down<1600> &allocator)
{
    int *i = allocator.allocate<int>(100);
    double *d = allocator.allocate<double>(100);
    char *c = allocator.allocate<char>(100);
    short *s = allocator.allocate<short>(100);
}

int main()
{
    // Bump up Single Allocations r-value reference
    auto bench_single_allocations_bup = benchmark::run_benchmark(
        "Single Allocations (Bump Up (lambda: pass by r-value reference (&&)))",
        100,
        [](bump::bump_up<4096> &&allocator)
        {
            int *i = allocator.allocate<int>(1);
            *i = 42;
        },
        bump::bump_up<4096>()
    );
    std::cout << "Average time taken per run: " << bench_single_allocations_bup << "ns\n\n";

    // Bump Down Single Allocations r-value reference
    auto bench_single_allocations_bdown = benchmark::run_benchmark(
        "Single Allocations (Bump Down (lambda: pass by r-value reference (&&)))", 
        100, 
        [](bump::bump_down<4096> &&allocator)
        {
            int* i = allocator.allocate<int>(1);
            *i = 42; 
        },
        bump::bump_down<4096>()
    );
    std::cout << "Average time taken per run: " << bench_single_allocations_bdown << "ns\n\n";

    // Bump Up Big Allocations l-value reference
    auto bench_big_allocations_bup = benchmark::run_benchmark(
        "Big Allocations (Bump Up (lambda: pass by r-value reference (&&)))", 
        100, 
        [](bump::bump_up<1600> &&allocator)
        {
            int* i = allocator.allocate<int>(100);
            double* d = allocator.allocate<double>(100);
            char* c = allocator.allocate<char>(100);
            short* s = allocator.allocate<short>(100); 
        },
        bump::bump_up<1600>()
    );
    std::cout << "Average time taken per run: " << bench_big_allocations_bup << "ns\n\n";

    // Bump Down Big Allocations l-value reference
    auto bench_big_allocations_bdown = benchmark::run_benchmark(
        "Big Allocations (Bump Down (lambda: pass by r-value reference (&&)))", 
        100, 
        [](bump::bump_down<1600> &&allocator)
        {
            int* i = allocator.allocate<int>(100);
            double* d = allocator.allocate<double>(100);
            char* c = allocator.allocate<char>(100);
            short* s = allocator.allocate<short>(100); 
        },
        bump::bump_down<1600>()
    );
    std::cout << "Average time taken per run: " << bench_big_allocations_bdown << "ns\n\n";

    // Bump Uo Mixed Size Allocations void function(void)
    auto bench_mixed_size_allocations_bup = benchmark::run_benchmark("Mixed Size Allocations (Bump Up (void function(void)))", 100, MixedSizeAllocationsBumpUp);
    std::cout << "Average time taken per run: " << bench_mixed_size_allocations_bup << "ns\n\n";

    // Bump DOwn Mixed Size Allocations void function(void)
    auto bench_mixed_size_allocations_bdown = benchmark::run_benchmark("Mixed Size Allocations (Bump Down (void function(void)))", 100, MixedSizeAllocationsBumpDown);
    std::cout << "Average time taken per run: " << bench_mixed_size_allocations_bdown << "ns\n\n";

    // Bump Up Loop Allocations and Deallocation l-value reference
    bump::bump_up<4096> bup_allocator;
    auto bench_loop_allocations_and_deallocation_bup = benchmark::run_benchmark("Loop Allocations and Deallocation (Bump Up (void function(pass by l-value ref (&))))", 1, LoopAllocationAndDeallocationBumpUp, bup_allocator);
    std::cout << "Average time taken per run: " << bench_loop_allocations_and_deallocation_bup << "ns\n\n";

    // Bump Up Loop Allocations and Deallocation r-value reference
    auto bench_rvr_loop_alloc_and_dealloc_bup = benchmark::run_benchmark(
        "Loop Allocations and Deallocation (Bump Up (lambda: pass by rvr))", 
        1, 
        [](bump::bump_up<4096> &&allocator)
        {
            for (int i = 0; i < 100; ++i) {
                allocator.allocate<int>(1);
            }
            allocator.deallocate();
        },
        bump::bump_up<4096>()
    );
    std::cout << "Average time taken per run: " << bench_rvr_loop_alloc_and_dealloc_bup << "ns\n\n";

    // Bump Down Loop Allocations and Deallocation l-value reference
    bump::bump_down<4096> bdown_allocator;
    auto bench_loop_allocations_and_deallocation_bdown = benchmark::run_benchmark("Loop Allocations and Deallocation (Bump Down (void function(pass by l-value ref (&))))", 1, LoopAllocationAndDeallocationBumpDown, bdown_allocator);
    std::cout << "Average time taken per run: " << bench_loop_allocations_and_deallocation_bdown << "ns\n\n";

    // Bump Down Loop Allocations and Deallocation r-value reference
    auto bench_rvr_loop_alloc_and_dealloc_bdown = benchmark::run_benchmark(
        "Loop Allocations and Deallocation (Bump Down (lambda: pass by rvr))", 
        1, 
        [](bump::bump_down<4096> &&allocator)
        {
            for (int i = 0; i < 100; ++i) {
                allocator.allocate<int>(1);
            }
            allocator.deallocate(); 
        },
        bump::bump_down<4096>()
    );
    std::cout << "Average time taken per run: " << bench_rvr_loop_alloc_and_dealloc_bdown << "ns\n\n";
}

void MixedSizeAllocationsBumpUp(void)
{
    bump::bump_up<4096> allocator;

    struct A
    {
        int a;
        double b;
        char c;
    };
    int *i = allocator.allocate<int>(20);
    double *d = allocator.allocate<double>(200);
    char *c = allocator.allocate<char>(3);
    struct A *st = allocator.allocate<struct A>(50);

    *i = 42;
    *d = 3.14;
    *c = 'A';
    st->a = 42;
    st->b = 3.14;
    st->c = 'A';
}
void MixedSizeAllocationsBumpDown(void)
{
    bump::bump_down<4096> allocator;
    struct A
    {
        int a;
        double b;
        char c;
    };
    int *i = allocator.allocate<int>(20);
    double *d = allocator.allocate<double>(200);
    char *c = allocator.allocate<char>(3);
    struct A *st = allocator.allocate<struct A>(50);

    *i = 42;
    *d = 3.14;
    *c = 'A';
    st->a = 42;
    st->b = 3.14;
    st->c = 'A';
}

void LoopAllocationAndDeallocationBumpUp(bump::bump_up<4096> &allocator)
{
    for (int i = 0; i < 100; ++i)
    {
        allocator.allocate<int>(1);
    }
    allocator.deallocate();
}

void LoopAllocationAndDeallocationBumpDown(bump::bump_down<4096> &allocator)
{
    for (int i = 0; i < 100; ++i)
    {
        allocator.allocate<int>(1);
    }
    allocator.deallocate();
}