#pragma once

#include <iostream>
#include <stdexcept>

template <std::size_t S>
class bump
{

    typedef char byte;

public:
    bump()
    {
        if (S < 1)
        {
            throw std::invalid_argument("Invalid. Size must be greater than 0.");
        }

        pool_size = S;
        pool = new byte[pool_size];
        next = pool;
    }

    ~bump() { deallocate(); }

    template <class T>
    T *allocate(std::size_t n)
    {
        if (n < 1)
            return nullptr;

        if (pool == nullptr)
        {
            pool = new byte[pool_size];
            next = pool;
        }

        std::size_t bytes_needed = sizeof(T) * n;
        std::size_t alignment = alignof(T);
        // std::size_t space_needed = bytes_needed + alignment - 1;

        // if (pool + pool_size - next < space_needed)
        //     return nullptr;

        std::uintptr_t raw_address = reinterpret_cast<std::uintptr_t>(next);
        std::uintptr_t mask = alignment - 1;
        std::uintptr_t aligned_address = (raw_address + mask) & ~mask;

        if (reinterpret_cast<byte *>(aligned_address + bytes_needed) > pool + pool_size)
            return nullptr;

        next = reinterpret_cast<byte *>(aligned_address + bytes_needed);

        return reinterpret_cast<T *>(aligned_address);
    }

    void deallocate()
    {
        if (pool)
        {
            delete[] pool;
            pool = nullptr;
            next = nullptr;
        }
    }

    void print_next_addr() const
    {
        std::cout << "Address: " << reinterpret_cast<std::uintptr_t>(next) << std::endl;
    }

private:
    byte *pool;
    byte *next;
    std::size_t pool_size;
}; // CLASS bump