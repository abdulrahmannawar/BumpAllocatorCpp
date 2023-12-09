#include "bump.h"
#include <iostream>

int main()
{
    bump<30> allocator;

    std::cout << "Before allocation: ";
    allocator.print_next_addr();

    char *ch = allocator.allocate<char>(1);
    std::cout << "After allocating char: ";
    allocator.print_next_addr();

    double *d = allocator.allocate<double>(1);
    std::cout << "After allocating double: ";
    allocator.print_next_addr();

    short *sh = allocator.allocate<short>(1);
    std::cout << "After allocating short: ";
    allocator.print_next_addr();

    char *ch1 = allocator.allocate<char>(1);
    std::cout << "After allocating char: ";
    allocator.print_next_addr();

    int *i = allocator.allocate<int>(1);
    std::cout << "After allocating int: ";
    allocator.print_next_addr();

    if (!i)
    {
        std::cerr << "Error: Failed to allocate int i.\n";
    }
}
