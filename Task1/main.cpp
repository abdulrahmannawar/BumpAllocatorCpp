#include "bump.hpp"
#include <iostream>

int main()
{
    bump<20> allocator;

    std::cout << "Address before allocation: ";
    allocator.print_addr();
    
    char* ch = allocator.allocate<char>(1);
    std::cout << "Address after allocating char: ";
    allocator.print_addr();

    short* sh = allocator.allocate<short>(1);
    std::cout << "Address after allocating short: ";
    allocator.print_addr();

    int* i = allocator.allocate<int>(1);
    std::cout << "Address after allocating int: ";
    allocator.print_addr();
}
