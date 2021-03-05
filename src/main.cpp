#include "yvm.hpp"

inline void log_yvm_usage() noexcept {
    LOG("Usage:\n");
    LOG("\t<path> to run program\n");
}

int main(int argc, char **argv) {
    // Check file path and VM configs
    char *filePath;
    if (argc == 2) {
        filePath = argv[1];
    } else {
        log_yvm_usage();
        return 0;
    }

    // Run VM
    try {
        Yrin::VM::init_table();
        Yrin::VM vm;
        vm.read(filePath);
        vm.run();
    } catch (Yrin::Error::YvmException &exception) {
        ERROR_LOG("VM crashed. Error code %d\n", exception.errorCode);
    } catch (std::exception &exception) {
        ERROR_LOG("Unexpected exception thrown.\n%s\n", exception.what());
    }
    return 0;
}
