#pragma once

#ifndef YRINVM_YRIN_STACK_HPP
#define YRINVM_YRIN_STACK_HPP

#include "yrin_defs.hpp"
#include <vector>
#include <stack>

namespace Yrin::Memory {

    // TODO: Organize into proper class and make it dynamic
    // Type ids
    namespace TypeId {
        constexpr int Undefined = 0;
        constexpr int Integer = 1;
        constexpr int LongLong = 2;
        constexpr int Floating = 3;
        constexpr int Double = 4;
        constexpr int Boolean = 5;
        constexpr int Character = 6;
    }

    // Custom memory management
    class StackMemory {
    public:
        // Define a struct which contains element type data
        struct ElementType {
            size_t size = 0;
        };

        // Define a struct which contains element value
        union ElementValue {
            int i;
            long long ll;
            float f;
            double d;
            bool b;
            char c;
            void *ptr;
        };

        // Represent an element in memory
        struct Element {
            // Type index
            int type;
            // Data
            ElementValue value;
            // Default constructor
            Element(): value{.i = 0}, type(TypeId::Undefined) {}
            // Convenience constructors
            explicit Element(int i) : value{.i = i}, type(TypeId::Integer) {}
            explicit Element(long long ll) : value{.ll = ll}, type(TypeId::Integer) {}
            explicit Element(float f) : value{.f = f}, type(TypeId::Integer) {}
            explicit Element(double d) : value{.d = d}, type(TypeId::Integer) {}
            explicit Element(bool b) : value{.b = b}, type(TypeId::Integer) {}
            explicit Element(char c) : value{.c = c}, type(TypeId::Integer) {}
            explicit Element(void *ptr, int type) : value{.ptr = ptr}, type(type) {}
            // TODO: Si può fare meglio ?
            // Convenience function to retrieve data
            template<typename T>
            [[nodiscard]] inline T &data() const noexcept { return *reinterpret_cast<T*>((void*)&value); }
            // Convenience function to retrieve element data size
            [[nodiscard]] inline size_t size() const noexcept { return TypeTable[type].size; }
        };

    private:
        // Types table
        static ElementType TypeTable[256];

        // Memory load index
        int memoryIndex = 0;

        // TODO: dynamic memory
        // Memory
        Element memory[1024];

    public:
        // Initialize types table
        static void init_table() noexcept;

        // Push element into Memory
        template<typename T>
        inline void push(T t) noexcept { memory[memoryIndex++] = Element(t); }
        inline void push(void *ptr, int type) noexcept { memory[memoryIndex++] = Element(ptr, type); }
        inline void push(const Element &e) noexcept { memory[memoryIndex++] = e; }

        // Pop element from Memory
        inline const Element &pop() noexcept { return memory[--memoryIndex]; }

        // Get element from Memory at index (from stack's head)
        inline const Element &get(int index) noexcept { return memory[memoryIndex - index - 1]; }
    };

} // Yrin

#endif //YRINVM_YRIN_STACK_HPP
