#include "yrin_vm.hpp"

// mem copy
#include <cstring>

void Yrin::VM::run() {
    // TODO: exception handling
    // Main loop
    while (!ips.empty()) {
        BYTE instruction = code[ips.top()++];
        OpTable[instruction](*this);
    }
}

void Yrin::VM::ret() noexcept {
    // TODO: error check
    ips.pop();
}

BYTE *Yrin::VM::next(size_t size) noexcept {
    // TODO: error check
    BYTE *ptr = &code[ips.top()];
    ips.top() += size;
    return ptr;
}

void Yrin::VM::_push(void *data, size_t size) noexcept {
    // TODO: error check
    memcpy(&rntstack[rntstack_load_index], data, size);
    rs_offsets.push_back(rntstack_load_index);
    rntstack_load_index += size;
}

void *Yrin::VM::pop() noexcept {
    // TODO: error check
    rntstack_load_index = rs_offsets.back();
    rs_offsets.pop_back();
    return &rntstack[rntstack_load_index];
}

void Yrin::VM::store() noexcept {
    // TODO: error check
    int s = rs_offsets.back();
    int sz = rntstack_load_index - s;
    rs_offsets.pop_back();
    memcpy(&datastack[datastack_load_index], &rntstack[rntstack_load_index], sz);
    ds_indexes.push_back(rntstack_load_index);
    rntstack_load_index += sz;
}

void Yrin::VM::load(int index) noexcept {
    // TODO: error check
    int s = ds_indexes[ds_indexes.size() - index - 2];
    int e = ds_indexes[ds_indexes.size() - index - 1];
    _push(&datastack[s], (e - s));
}
