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
