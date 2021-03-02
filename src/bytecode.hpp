#pragma once

#ifndef YRINVM_BYTECODE_HPP
#define YRINVM_BYTECODE_HPP

namespace YVM::Bytecode {

    constexpr int YVM_CODE_VERSION_MAJOR = 0;
    constexpr int YVM_CODE_VERSION_MINOR = 1;
    constexpr int YVM_CODE_VERSION_TYPE = 'b';
    constexpr int YVM_CODE_VERSION_RELEASE = 1;

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

    // Bytecode metadata
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

    // Flags list
    namespace InstructionFlags {
        constexpr int NUMBER_TYPE_FLAG = 1 << 0; // DECIMAL (0) or INTEGER (1)
    }

    // Bytecode instruction
    struct Instruction {
        int opcode: 8;
        int flags: 24;
        union {
            int data_as_int;
            float data_as_float;
        };
        // Flags utils
        [[nodiscard]] inline int data_is_int() const noexcept { return (flags & InstructionFlags::NUMBER_TYPE_FLAG); }
        [[nodiscard]] inline int data_is_dec() const noexcept { return !(flags & InstructionFlags::NUMBER_TYPE_FLAG); }
    };

    // Bytecode data
    struct Data {
        Version version;
        std::vector<Instruction> instructions;
    };

} // YVM

#endif //YRINVM_BYTECODE_HPP
