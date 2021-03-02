#pragma once

#ifndef YRINVM_YVMEXCEPTION_HPP
#define YRINVM_YVMEXCEPTION_HPP

namespace YVM::Exception {

    // Error list
    namespace ERRORS {
        constexpr int INVALID_CMD_LINE_ARGUMENT = 0x001;
        constexpr int UNABLE_TO_OPEN_FILE = 0x002;
        constexpr int VERSION_NOT_SUPPORTED = 0x003;
        constexpr int POP_FROM_EMPTY_STACK = 0x004;
        constexpr int INDEX_OUT_OF_BOUNDS = 0x005;
        constexpr int WRONG_ARGUMENT_TYPE = 0x006;
    };

    // Generic YVM Exception
    struct YvmException : public std::exception {
        const int errorCode;
        explicit YvmException(int code) : errorCode(code) {}
    };

    // Exception generated from the Loader
    struct YvmLoaderException : public YvmException {
        explicit YvmLoaderException(int code) : YvmException(code) {}
    };

    // Exception generated from the Executor
    struct YvmExecutorException : public YvmException {
        explicit YvmExecutorException(int code) : YvmException(code) {}
    };

} // YVM::Exception

#endif //YRINVM_YVMEXCEPTION_HPP
