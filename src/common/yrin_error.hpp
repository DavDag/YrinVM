#pragma once

#ifndef YRINVM_YRIN_ERROR_HPP
#define YRINVM_YRIN_ERROR_HPP

#include "../common/yrin_defs.hpp"
#include <exception>

namespace Yrin::Error {

    // Error list
    namespace Code {
        constexpr int UNABLE_TO_OPEN_FILE = 0x001;
        constexpr int VERSION_NOT_SUPPORTED = 0x002;
        constexpr int POP_FROM_EMPTY_STACK = 0x003;
        constexpr int INDEX_OUT_OF_BOUNDS = 0x004;
        constexpr int WRONG_ARGUMENT_TYPE = 0x005;
    } // Code

    // YVM Exception
    struct YvmException : public std::exception {
        const int errorCode;
        explicit YvmException(int code) : errorCode(code) {}
    };

} // Yrin::Error

#endif //YRINVM_YRIN_ERROR_HPP
