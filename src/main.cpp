#include "common.hpp"
#include "loader.hpp"
#include "executor.hpp"

inline void log_yvm_usage() noexcept {
    LOG("Usage:\n");
    LOG("\t<path> to run program\n");
}

int main(int argc, char **argv) {
    // Load file path and VM configs
    char *filePath;
    if (argc == 2) {
        filePath = argv[1];
    } else {
        log_yvm_usage();
        return 0;
    }

    // VM loop
    try {
        // Load code
        const YVM::Bytecode::Data &code = YVM::Loader::load(filePath);
        // Execute it
        YVM::Executor::execute(code);
    } catch (YVM::Exception::YvmLoaderException &exception) {
        ERROR_LOG("VM loader crashed. Error code %d\n", exception.errorCode);
    } catch (YVM::Exception::YvmExecutorException &exception) {
        ERROR_LOG("VM executor crashed. Error code %d\n", exception.errorCode);
    } catch (std::exception &exception) {
        ERROR_LOG("VM crashed due to unhandled exception: %s\n", exception.what());
    }
    return 0;
}
