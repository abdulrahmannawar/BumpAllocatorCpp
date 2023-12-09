#pragma once

#include <iostream>
#include <stdexcept>

// Templated class for a bump allocator
template <std::size_t S>
class bump
{
    typedef char byte;

public:
    // Constructor
    bump()
    {
        // Check if size is valid
        if (S < 1)
        {
            throw std::invalid_argument("Invalid. Size must be greater than 0.");
        }

        // Initialize pool size and pointers
        pool_size = S;
        pool = new byte[pool_size];
        next = pool;
    }

    // Destructor
    ~bump() { deallocate(); }

    // Allocate memory for type T
    template <class T>
    T *allocate(std::size_t n)
    {
        // Check if allocation size is valid
        if (n < 1)
            return nullptr;

        // Allocate pool if not initialized
        if (pool == nullptr)
        {
            pool = new byte[pool_size];
            next = pool;
        }

        // Calculate required bytes and alignment
        std::size_t bytes_needed = sizeof(T) * n;
        std::size_t alignment = alignof(T);

        // Calculate aligned address
        std::uintptr_t raw_address = reinterpret_cast<std::uintptr_t>(next);
        std::uintptr_t mask = alignment - 1;

        // Ensure alignment by rounding up the raw address to the next multiple of the required alignment
        std::uintptr_t aligned_address = (raw_address + mask) & ~mask;

        // Check if allocation exceeds pool size
        if (reinterpret_cast<byte *>(aligned_address + bytes_needed) > pool + pool_size)
            return nullptr;

        // Update next pointer and return the aligned address
        next = reinterpret_cast<byte *>(aligned_address + bytes_needed);
        return reinterpret_cast<T *>(aligned_address);
    }

    // Deallocate memory
    void deallocate()
    {
        if (pool)
        {
            delete[] pool;
            pool = nullptr;
            next = nullptr;
        }
    }

    // Print the next address in the pool
    void print_next_addr() const
    {
        std::cout << "Address: " << reinterpret_cast<std::uintptr_t>(next) << std::endl;
    }

private:
    // Private members
    byte *pool;
    byte *next;
    std::size_t pool_size;
}; // CLASS bump