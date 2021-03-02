#pragma once

#ifndef YRINVM_COMMON_HPP
#define YRINVM_COMMON_HPP

#define YVM_CODE_VERSION_MAJOR 0
#define YVM_CODE_VERSION_MINOR 1
#define YVM_CODE_VERSION_TYPE 'b'
#define YVM_CODE_VERSION_RELEASE 1

#include <cstdio>
#include <cstdint>
#include <vector>
#include "yvmexception.hpp"

#define ERROR_LOG(...) fprintf(stderr, __VA_ARGS__)

#ifndef NDEBUG
#define DEBUG_LOG(...) fprintf(stdout, __VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#ifdef LOADER_LOG
#define LOADER_DEBUG_LOG(...) DEBUG_LOG(__VA_ARGS__)
#else
#define LOADER_DEBUG_LOG(...)
#endif

#ifdef EXECUTOR_LOG
#define EXECUTOR_DEBUG_LOG(...) DEBUG_LOG(__VA_ARGS__)
#else
#define EXECUTOR_DEBUG_LOG(...)
#endif

namespace YVM::CODE {

    // Version X.Y.Z-W
    struct Version {
        int major: 8;   // X
        int minor: 8;   // Y
        int type: 8;    // Z
        int release: 8; // W

        // Operator overloading for equals
        bool operator!=(const Version &rhs) const {
            return major != rhs.major || minor != rhs.minor || type != rhs.type || release != rhs.release;
        }
    };

    // Metadata
    struct Metadata {
        Version version;
        int _padding: 32;
    };

    // Current YVM version
    constexpr Version CURRENT_VERSION = {
            .major = YVM_CODE_VERSION_MAJOR,
            .minor = YVM_CODE_VERSION_MINOR,
            .type = YVM_CODE_VERSION_TYPE,
            .release = YVM_CODE_VERSION_RELEASE
    };

    namespace InstructionFlags {
        constexpr int IS_INTEGER_FLAG = 1 << 0;
        constexpr int IS_DECIMAL_FLAG = 1 << 1;
    }

    // Instruction
    struct Instruction {
        int opcode: 8;
        int flags: 24;
        union {
            int data_as_int;
            float data_as_float;
        };
        // Flags
        [[nodiscard]] inline int data_is_int() const noexcept { return flags & InstructionFlags::IS_INTEGER_FLAG; }
        [[nodiscard]] inline int data_is_dec() const noexcept { return flags & InstructionFlags::IS_DECIMAL_FLAG; }
    };

    // CODE representation
    struct Data {
        Version version;
        std::vector<Instruction> instructions;
    };

} // YVM

#endif //YRINVM_COMMON_HPP
