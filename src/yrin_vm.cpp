#include "yvm.hpp"

namespace Yrin {

    void VM::run() {
        // TODO: error handling
        // Main loop
        while (!ips.empty()) {
            const BYTE& instruction = code[ips.top()++];
            OpTable[instruction](*this);
        }
    }

    void VM::ret() noexcept {
        // TODO: error check
        ips.pop();
    }

    BYTE *VM::next(size_t size) noexcept {
        // TODO: error check
        BYTE *ptr = &code[ips.top()];
        ips.top() += size;
        return ptr;
    }

} // Yrin
