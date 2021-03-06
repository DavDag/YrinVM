#pragma once

#ifndef YRINVM_YRIN_VM_HPP
#define YRINVM_YRIN_VM_HPP

#include <vector>
#include <stack>

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

        // Return from function call
        void ret() noexcept;

        // Retrieve BYTEs from code
        BYTE *next(size_t) noexcept;

        // Push data into RuntimeStack
        inline void push(int i) noexcept { rntStack.push(i); }
        inline void push(long long ll) noexcept { rntStack.push(ll); }
        inline void push(float f) noexcept { rntStack.push(f); }
        inline void push(double d) noexcept { rntStack.push(d); }
        inline void push(char c) noexcept { rntStack.push(c); }
        inline void push(bool b) noexcept { rntStack.push(b); }
        inline void push(void* ptr, int type) noexcept { rntStack.push(ptr, type); }

        // Retrieve data from RuntimeStack
        template<typename T>
        inline int pop() noexcept { return rntStack.pop().data<T>(); }

        // Remove data from RuntimeStack
        inline void pop() noexcept { rntStack.pop(); }

        // Push data into DataStack
        inline void store() noexcept { dataStack.push(rntStack.pop()); }

        // Retrieve data from DataStack
        inline void load(int index) noexcept { rntStack.push(dataStack.get(index)); }
    };

} // Yrin

#endif //YRINVM_YRIN_VM_HPP
