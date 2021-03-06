#include "yrin_stack.hpp"

using namespace Yrin::Memory;

void StackMemory::push(void *data, size_t size) noexcept {
    // Copy data into memory
    memcpy(&_memory[_index], data, size);

    // Increment index
    _index += size;

    // Save index
    _data.push_back(_index);
}

const StackMemory::Element &StackMemory::pop() noexcept {
    static StackMemory::Element element{.ptr = nullptr, .size = 0};

    // Calc top element begin index in memory (s)
    int &s = _data[_data.size() - 2];

    // Calc top element end index in memory (e)
    int &e = _data[_data.size() - 1];

    // Decrement index
    _index -= (e - s);

    // Delete saved index
    _data.pop_back();

    // Return element
    element.ptr = &_memory[_index];
    element.size = (e - s);
    return element;
}

const StackMemory::Element &StackMemory::get(int index) noexcept {
    static StackMemory::Element element{.ptr = nullptr, .size = 0};

    // Calc element begin index in memory (s)
    int &s = _data[_data.size() - index - 2];

    // Calc element end index in memory (e)
    int &e = _data[_data.size() - index - 1];

    // Return element
    element.ptr = &_memory[s];
    element.size = (e-s);
    return element;
}
