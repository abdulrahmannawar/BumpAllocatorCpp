#pragma once

#include <iostream>

typedef char byte;

template <std::size_t S>
class bump {
public:
    bump() {
        pool_size = S;
        pool = new byte[pool_size];
        next = pool;
    }

    explicit bump(std::size_t size) {
        pool_size = size;
        pool = new byte[pool_size];
        next = pool;
    }

    ~bump() { delete[] pool; }

    template <class T>
    T *allocate(std::size_t n) {
        std::size_t alignment = alignof(T);
        // std::cout << "Alignment Size: " << alignment << std::endl;
        // std::cout << "Address % alignment: " << (reinterpret_cast<std::size_t>(next) % alignment) << std::endl;
        // std::cout << "Alignment - (Address % alignement): " << (alignment - (reinterpret_cast<std::size_t>(next) % alignment)) << std::endl;
        // std::cout << "(Alignment - (Address % alignment)) % Alignment: " << ((alignment - (reinterpret_cast<std::size_t>(next) % alignment)) % alignment) << std::endl;
        std::size_t adjustment = (alignment - (reinterpret_cast<std::size_t>(next) % alignment)) % alignment;

        if (pool + S - next < (sizeof(T) * n) + adjustment)
            return nullptr;

        byte *aligned = next + adjustment;

        T *allocated = reinterpret_cast<T*>(aligned);
        next = aligned + (sizeof(T) * n);
        return allocated;
    }

    void deallocate() {
        delete[] pool;
        pool = new byte[pool_size];
        next = pool;
    }

    template <class T>
    void testAlignment() {
        std::cout << "Before allocation:" << std::endl;
        print_addr();

        T *ptr = allocate<T>(sizeof(T));

        std::cout << "After allocation:" << std::endl;
        print_addr();

        print_addr(ptr);

        // deallocate();

        // std::cout << "After deallocation:" << std::endl;
        // print_addr();
        std::cout << std::endl;
    }

    template <class T>
    void print_addr(T *ptr) {
        std::cout << "Allocated memory address for type " << typeid(T).name() << ": " << reinterpret_cast<std::size_t>(ptr) << std::endl;
    }

    void print_addr() {
        std::cout << "Address: " << reinterpret_cast<std::size_t>(next) << std::endl;
    }


    /// get address and value
    template <class T>
    void print_addr_val(T *ptr) {
        std::cout << "Address: " << reinterpret_cast<std::size_t>(ptr) << std::endl;
        std::cout << "Value: " << *ptr << std::endl;
    }

private:
    byte *pool;
    byte *next;
    std::size_t pool_size;
};