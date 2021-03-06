#pragma once

#ifndef YRINVM_YRIN_STACK_HPP
#define YRINVM_YRIN_STACK_HPP

#include "../common/yrin_defs.hpp"
#include <vector>
#include <stack>

namespace Yrin::Memory {

    // Custom memory management
    class StackMemory {
    public:
        struct Element {
            void *ptr;
            int size;
            template<typename T>
            inline T* data() noexcept { return reinterpret_cast<T*>(ptr); }
        };

    private:
        int _index = 0;
        BYTE _memory[1024] = {0};
        std::vector<int> _data = {0};

    public:
        // Push element into Memory
        void push(void *, size_t) noexcept;

        // Pop element from Memory
        const Element& pop() noexcept;

        // Get element from Memory at index (from stack's head)
        const Element& get(int) noexcept;
    };

} // Yrin

#endif //YRINVM_YRIN_STACK_HPP
