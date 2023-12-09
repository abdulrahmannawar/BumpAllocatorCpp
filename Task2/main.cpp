#include "bump.h"
#include "./simpletest/simpletest.h"
#include <iostream>
#include <stdexcept>
#include <cstdint>

const char *group = "Bump";

DEFINE_TEST_G(FullAllocatorTest, Bump)
{
    bump<20 * sizeof(int)> bumper;

    int *x = bumper.allocate<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 10 ints.");

    int *y = bumper.allocate<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate 10 ints.");

    int *z = bumper.allocate<int>(10);
    TEST_MESSAGE(z == nullptr, "Allocated 10 ints when there was not enough space.");
}

DEFINE_TEST_G(ZeroSizedAllocatorTest, Bump)
{
    bool exception_thrown = false;

    try
    {
        bump<0> bumper;
    }
    catch (const std::exception &e)
    {
        exception_thrown = true;
    }

    TEST_MESSAGE(exception_thrown, "Failed to throw exception when size is 0.");
}

DEFINE_TEST_G(AllocatingZeroTest, Bump)
{
    bump<20 * sizeof(int)> bumper;

    int *x = bumper.allocate<int>(0);
    TEST_MESSAGE(x == nullptr, "Allocated 0 ints.");
}

DEFINE_TEST_G(OversizedAllocatorTest, Bump)
{
    bool exception_thrown = false;

    try
    {
        bump<UINT64_MAX> bumper;
    }
    catch (const std::exception &e)
    {
        exception_thrown = true;
    }

    TEST_MESSAGE(exception_thrown, "Failed to throw exception when allocator size is too large.");
}

DEFINE_TEST_G(DeallocateTest, Bump)
{
    bump<20 * sizeof(int)> bumper;

    int *x = bumper.allocate<int>(20);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 20 ints.");

    bumper.deallocate();

    int *y = bumper.allocate<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate 10 ints after deallocation.");
}

DEFINE_TEST_G(MixedAllocationsAndDeallocationTest, Bump)
{
    bump<20 * sizeof(int)> bumper;

    int *x = bumper.allocate<int>(5);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 5 ints.");

    char *y = bumper.allocate<char>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate 10 chars.");

    bumper.deallocate();

    double *z = bumper.allocate<double>(11);
    TEST_MESSAGE(z == nullptr, "Allocated 11 doubles after deallocation and exceed size limit.");

    double *w = bumper.allocate<double>(10);
    TEST_MESSAGE(w != nullptr, "Failed to allocate 10 doubles after deallocation.");
}

DEFINE_TEST_G(AlignmentAndMultipleDataTypesTest, Bump)
{
    bump<20 * sizeof(int)> bumper;

    int *x = bumper.allocate<int>(1);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 1 int.");

    double *y = bumper.allocate<double>(1);
    TEST_MESSAGE(y != nullptr, "Failed to allocate 1 double.");

    short *z = bumper.allocate<short>(1);
    TEST_MESSAGE(z != nullptr, "Failed to allocate 1 short.");

    char *w = bumper.allocate<char>(1);
    TEST_MESSAGE(w != nullptr, "Failed to allocate 1 char.");

    TEST_MESSAGE(reinterpret_cast<std::uintptr_t>(x) % alignof(int) == 0, "First: int is not aligned.");
    TEST_MESSAGE(reinterpret_cast<std::uintptr_t>(y) % alignof(double) == 0, "Second: double is not aligned.");
    TEST_MESSAGE(reinterpret_cast<std::uintptr_t>(z) % alignof(short) == 0, "Third: short is not aligned.");
    TEST_MESSAGE(reinterpret_cast<std::uintptr_t>(w) % alignof(char) == 0, "Fourth: char is not aligned.");
}

DEFINE_TEST_G(MassiveAllocationTest, Bump)
{
    constexpr std::size_t _4GB = 4ULL << 30;

    bump<_4GB> bumper;

    char *x = bumper.allocate<char>(_4GB);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 4GB of memory.");
}

DEFINE_TEST_G(StructAllocationTest, Bump)
{
    struct test_struct
    {
        int x;
        double y;
        char z;
    };

    bump<10 * sizeof(struct test_struct)> bumper;

    test_struct *x = bumper.allocate<struct test_struct>(5);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 5 test_structs.");
    TEST_MESSAGE(reinterpret_cast<std::uintptr_t>(x) % alignof(struct test_struct) == 0, "test_struct is not aligned.");

    test_struct *y = bumper.allocate<struct test_struct>(6);
    TEST_MESSAGE(y == nullptr, "Failed to allocate 6 test_structs.");
}

DEFINE_TEST_G(UnionAllocationTest, Bump)
{
    union test_union
    {
        int x;
        double y;
        char z;
    };

    bump<10 * sizeof(union test_union)> bumper;

    test_union *x = bumper.allocate<union test_union>(5);
    TEST_MESSAGE(x != nullptr, "Failed to allocate 5 test_unions.");
    TEST_MESSAGE(reinterpret_cast<std::uintptr_t>(x) % alignof(union test_union) == 0, "test_union is not aligned.");

    test_union *y = bumper.allocate<union test_union>(6);
    TEST_MESSAGE(y == nullptr, "Failed to allocate 6 test_unions.");
}

DEFINE_TEST_G(MemoryLeakDetectionTest, Bump)
{
    {
        bump<10 * sizeof(int)> bumper;
        std::cout << "\nAddress (before allocation): ";
        bumper.print_next_addr();
        int *x = bumper.allocate<int>(10);
        std::cout << "Address (after allocation of 10 ints): ";
        bumper.print_next_addr();
        TEST_MESSAGE(x != nullptr, "Failed to allocate 10 ints.");
    }

    bump<10 * sizeof(int)> bumper;
    std::cout << "Address (after destruction): ";
    bumper.print_next_addr();
    int *y = bumper.allocate<int>(10);
    std::cout << "Address (after allocation of 10 ints): ";
    bumper.print_next_addr();
    TEST_MESSAGE(y != nullptr, "Failed to allocate 10 ints after destruction (exiting scope).");
}

DEFINE_TEST_G(RepeatedAllocatorDestructionAndCreationTest, Bump)
{
    for (int i = 0; i < 5; i++)
    {
        bump<20 * sizeof(int)> bumper;

        int *x = bumper.allocate<int>(20);
        TEST_MESSAGE(x != nullptr, "Failed to allocate 20 ints.");
    }
}

int main()
{
    bool pass = true;

    pass &= TestFixture::ExecuteTestGroup(group, TestFixture::OutputMode::Verbose);

    return pass ? 0 : 1;
}