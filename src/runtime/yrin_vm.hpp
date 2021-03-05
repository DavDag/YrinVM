#pragma once

#ifndef YRINVM_YRIN_VM_HPP
#define YRINVM_YRIN_VM_HPP

#include <vector>
#include <stack>
#include "../common/yrin_defs.hpp"
#include "yrin_datatypes.hpp"

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

        // TODO: Create custom stacks
        // Runtime stack
        int rntstack_load_index = 0;
        BYTE rntstack[1024];
        std::vector<int> rs_offsets; // include type flag inside to dump
        // Data stack
        int datastack_load_index = 0;
        BYTE datastack[1024];
        std::vector<int> ds_indexes = {0}; // include type flag inside to dump

        // Push data into RuntimeStack
        void _push(void *, size_t) noexcept;

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
        inline void push(int i) noexcept { _push(reinterpret_cast<void *>(&i), sizeof(int)); }
        inline void push(bool b) noexcept { _push(reinterpret_cast<void *>(&b), sizeof(bool)); }
        inline void push(float f) noexcept { _push(reinterpret_cast<void *>(&f), sizeof(float)); }
        inline void push(double d) noexcept { _push(reinterpret_cast<void *>(&d), sizeof(double)); }
        inline void push(char c) noexcept { _push(reinterpret_cast<void *>(&c), sizeof(char)); }

        // Retrieve data from RuntimeStack
        void *pop() noexcept;

        // Push data into DataStack
        void store() noexcept;

        // Retrieve data from DataStack
        void load(int) noexcept;
    };

} // Yrin

#endif //YRINVM_YRIN_VM_HPP
