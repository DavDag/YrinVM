#pragma once

#ifndef YRINVM_YVMEXCEPTION_HPP
#define YRINVM_YVMEXCEPTION_HPP

namespace YVM::Exception {

    // Error list
    enum ERRORS : int {
        INVALID_CMD_LINE_ARGUMENT = 0x001,
        UNABLE_TO_OPEN_FILE = 0x002,
        VERSION_NOT_SUPPORTED = 0x003,
        POP_FROM_EMPTY_STACK = 0x004,
        INDEX_OUT_OF_BOUNDS = 0x005
    };

    class YvmException : public std::exception {
    public:
        const int errorCode;
        explicit YvmException(ERRORS errorCode) : errorCode(errorCode) {}
    };

    class YvmLoaderException : public YvmException {
    public:
        explicit YvmLoaderException(ERRORS errorCode) : YvmException(errorCode) {}
    };

    class YvmExecutorException : public YvmException {
    public:
        explicit YvmExecutorException(ERRORS errorCode) : YvmException(errorCode) {}
    };

} // YVM::Exception

#endif //YRINVM_YVMEXCEPTION_HPP
