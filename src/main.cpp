#include "yvm.hpp"

inline void log_yvm_server_usage() noexcept {
    LOG("Usage:\n");
    LOG("\t<path>: [REQUIRED] to run program\n");
}

int main(int argc, char **argv) {
    // Check file path and VM configs
    char *filePath;
    if (argc == 2) {
        filePath = argv[1];
    } else {
        log_yvm_server_usage();
        return 0;
    }

    // Data initialization
    Yrin::Memory::StackMemory::init_table();
    Yrin::VM::init_table();

    // VM
    Yrin::VM vm;
    vm.read(filePath);
    vm.run();
    return 0;
}
