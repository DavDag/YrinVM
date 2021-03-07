#pragma once

#ifndef YRINVM_YRIN_VM_HPP
#define YRINVM_YRIN_VM_HPP

#include <vector>
#include <stack>
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

    public:
        // Initialize operations table
        static void init_table() noexcept;

        // Load bytecode from file
        void read(const char *filePath);

        // Run VM
        void run();
        void run_server(int);

        // Return from function call
        void ret() noexcept;

        // Retrieve BYTEs from code
        BYTE *next(size_t) noexcept;

        // Push data into RuntimeStack
        template<typename T>
        inline void push(T t) noexcept { rntStack.push(t); }
        inline void push(void* ptr, int type) noexcept { rntStack.push(ptr, type); }

        // Retrieve data from RuntimeStack
        template<typename T>
        inline T pop() noexcept { return rntStack.pop().data<T>(); }

        // Remove data from RuntimeStack
        inline void pop() noexcept { rntStack.pop(); }

        // Push data into DataStack
        inline void store() noexcept { dataStack.push(rntStack.pop()); }

        // Retrieve data from DataStack
        inline void load(int index) noexcept { rntStack.push(dataStack.get(index)); }
    };

} // Yrin

#endif //YRINVM_YRIN_VM_HPP
