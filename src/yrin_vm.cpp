#include <yrin_vm.hpp>

#include "yvm.hpp"

namespace Yrin {

    void VM::run() {
        // TODO: error handling from operations
        try {
            // Main loop
            while (!ips.empty()) {
                const BYTE& instruction = code[ips.top()++];
                OpTable[instruction](*this);
            }
        } catch (Yrin::Error::YvmException &exception) {
            ERROR_LOG("VM crashed. Error code %d\n", exception.errorCode);
        } catch (std::exception &exception) {
            ERROR_LOG("Unexpected exception thrown.\n%s\n", exception.what());
        }
    }

    void VM::run_server(int port) {
        // TODO: Asio server setup

        try {
            // Main loop
            while (!ips.empty()) {
                const BYTE& instruction = code[ips.top()++];
                OpTable[instruction](*this);
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

    BYTE *VM::next(size_t size) noexcept {
        // TODO: error check
        BYTE *ptr = &code[ips.top()];
        ips.top() += size;
        return ptr;
    }

} // Yrin
