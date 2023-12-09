#pragma once

#include <iostream>
#include <stdexcept>

namespace bump
{
    typedef char byte;

    // Templated class for a bump_up allocator
    template <std::size_t S>
    class bump_up
    {

    public:
        // Constructor
        bump_up()
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
        ~bump_up() { deallocate(); }

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

            // Convert the next pointer to an unsigned integer representing the raw memory address
            std::uintptr_t raw_address = reinterpret_cast<std::uintptr_t>(next);

            // Calculate a mask to isolate the least significant bits that need adjustment for alignment
            // The mask is created by taking the value of alignment and subtracting 1
            std::uintptr_t mask = alignment - 1;

            // Calculate the aligned address by adding the mask to the raw address, rounding up
            // Then, apply a bitwise AND with the complement of the mask to clear the unnecessary bits
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
    }; // CLASS bump_up

    // Templated class for a bump down allocator
    template <std::size_t S>
    class bump_down
    {
    public:
        // Constructor
        bump_down()
        {
            // Check if size is valid
            if (S < 1)
            {
                throw std::invalid_argument("Invalid. Size must be greater than 0.");
            }

            // Initialize pool size and pointers
            pool_size = S;
            pool = new byte[pool_size];
            next = (pool + pool_size);
        }

        // Destructor
        ~bump_down() { deallocate(); }

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
                next = pool + pool_size;
            }

            // Calculate required bytes and alignment
            std::size_t bytes_needed = sizeof(T) * n;
            std::size_t alignment = alignof(T);
            std::size_t space_needed = bytes_needed + alignment - 1;

            // Check if there is enough space in the pool
            // if (next - pool < space_needed)
            //     return nullptr;

            // Convert the next pointer to an unsigned integer representing the raw memory address
            std::uintptr_t raw_address = reinterpret_cast<std::uintptr_t>(next);

            // Calculate a mask to isolate the least significant bits that need adjustment for alignment
            // The mask is created by inverting the bits of (alignment - 1)
            std::uintptr_t mask = ~(alignment - 1);

            // Calculate the aligned address by subtracting the required bytes and applying the mask
            // This ensures that the address is adjusted to the nearest lower multiple of the alignment
            std::uintptr_t aligned_address = (raw_address - bytes_needed) & mask;


            // Check if the aligned address is within the pool
            if (reinterpret_cast<byte *>(aligned_address) < pool)
                return nullptr;

            // Update next pointer and return the aligned address
            next = reinterpret_cast<byte *>(aligned_address);
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
    }; // CLASS bump_down

} // namespace bump