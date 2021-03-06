#pragma once

#ifndef YRINVM_YRIN_VM_HPP
#define YRINVM_YRIN_VM_HPP

#include <vector>
#include <stack>
#include "../common/yrin_defs.hpp"
#include "yrin_datatypes.hpp"
#include "yrin_stack.hpp"

namespace Yrin {

    // Main class representing the Yrin Virtual Machine
    class VM {
    private:
        // VM code to run
        std::vector<BYTE> code;

        // IP stack
        std::stack<int> ips;

        // Table of operations
        static op_callback OpTable[256];

        // Stacks
        Memory::StackMemory rntStack;
        Memory::StackMemory dataStack;

        // Push data into RuntimeStack
        inline void _push(void *data, size_t size) noexcept { rntStack.push(data, size); }

    public:
        // Initialize operations table
        static void init_table() noexcept;

        // Load bytecode from file
        void read(const char *filePath);

        // Run VM
        void run();

        // Return from function call
        void ret() noexcept;

        // Retrieve BYTEs from code
        BYTE *next(size_t) noexcept;

        // Push data into RuntimeStack
        template<typename T>
        inline void push(T t) noexcept {
            _push(&t, sizeof(T));
        }

        // Retrieve data from RuntimeStack
        inline Memory::StackMemory::Element pop() noexcept {
            return rntStack.pop();
        }

        // Push data into DataStack
        inline void store() noexcept {
            const auto &e = rntStack.pop();
            dataStack.push(e.ptr, e.size);
        }

        // Retrieve data from DataStack
        inline void load(int index) noexcept {
            const auto &e = dataStack.get(index);
            rntStack.push(e.ptr, e.size);
        }
    };

} // Yrin

#endif //YRINVM_YRIN_VM_HPP
