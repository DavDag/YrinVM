#include "yvm.hpp"

namespace Yrin {

    void VM::run() {
        // TODO: error handling from operations
        try {
            EXECUTOR_DEBUG_LOG("=================================\n"
                               "||          EXECUTOR           ||\n"
                               "=================================\n");
            // Main loop
            while (!ips.empty()) {
                int& instruction_index = ips.top();
                const BYTE &instruction = bytestream[code[instruction_index]];
                OpTable[instruction](*this);
                ++instruction_index;
            }
        } catch (Yrin::Error::YvmException &exception) {
            ERROR_LOG("VM crashed. Error code %d\n", exception.errorCode);
        } catch (std::exception &exception) {
            ERROR_LOG("Unexpected exception thrown.\n%s\n", exception.what());
        }
    }

    void VM::ret() noexcept {
        // TODO: error check
        ips.pop();
    }

    BYTE *VM::next(int &offset, size_t size) noexcept {
        // TODO: error check
        int ind = code[ips.top()] + 1; // shifted by 1 (opcode)
        BYTE *ptr = &bytestream[ind + offset];
        offset += size;
        return ptr;
    }

} // Yrin
