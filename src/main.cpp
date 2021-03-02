#include "common.hpp"
#include "loader.hpp"
#include "executor.hpp"

// 1st argument (argv[1]) must be a path to the file to read.
int main(int argc, char** argv) {
    try {
        const YVM::Bytecode::Data& code = YVM::Loader::load(argc, argv);
        YVM::Executor::execute(code);
    } catch (YVM::Exception::YvmLoaderException& exception) {
        ERROR_LOG("VM loader crashed. Error code %d\n", exception.errorCode);
    } catch (YVM::Exception::YvmExecutorException& exception) {
        ERROR_LOG("VM executor crashed. Error code %d\n", exception.errorCode);
    } catch (std::exception& exception) {
        ERROR_LOG("VM crashed due to unhandled exception: %s\n", exception.what());
    }
    return 0;
}
