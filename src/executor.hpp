#pragma once

#ifndef YRINVM_EXECUTOR_HPP
#define YRINVM_EXECUTOR_HPP

#include "common.hpp"

namespace YVM::Executor {

    // Runtime object type
    enum RntType : int {
        YVM_UNDEFINED = 0x0,
        YVM_BOOLEAN = 0x1,
        YVM_INTEGER = 0x2,
        YVM_DECIMAL = 0x3
    };

    // Runtime object value
    struct RntValue {
        union {
            int integer;
            bool boolean;
            float decimal;
        };
    };

    // Runtime object representation
    struct RntObj {
        const RntType type = RntType::YVM_UNDEFINED;
        const RntValue value = {.integer = 0};
        explicit RntObj(int value): type(RntType::YVM_INTEGER), value{.integer = value} { }
        explicit RntObj(float value) : type(RntType::YVM_DECIMAL), value{.decimal = value} { }
        explicit RntObj(bool value) : type(RntType::YVM_BOOLEAN), value{.boolean = value} { }
        [[nodiscard]] inline int integer() const noexcept { return value.integer; }
        [[nodiscard]] inline float decimal() const noexcept { return value.decimal; }
        [[nodiscard]] inline bool boolean() const noexcept { return value.boolean; }
    };

    // Execute code
    void execute(const Bytecode::Data &code);

} // YVM::Executer

#endif //YRINVM_EXECUTOR_HPP
