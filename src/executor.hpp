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
        explicit RntObj(int value) : type(RntType::YVM_INTEGER), value{.integer = value} {}
        explicit RntObj(float value) : type(RntType::YVM_DECIMAL), value{.decimal = value} {}
        explicit RntObj(bool value) : type(RntType::YVM_BOOLEAN), value{.boolean = value} {}
        [[nodiscard]] inline int integer() const noexcept { return value.integer; }
        [[nodiscard]] inline float decimal() const noexcept { return value.decimal; }
        [[nodiscard]] inline bool boolean() const noexcept { return value.boolean; }
        bool operator==(const RntObj &rhs) const {
            if (type != rhs.type) return false;
            if (type == RntType::YVM_UNDEFINED) return true;
            else if (type == RntType::YVM_BOOLEAN) return boolean() == rhs.boolean();
            else if (type == RntType::YVM_INTEGER) return integer() == rhs.integer();
            else if (type == RntType::YVM_DECIMAL) return decimal() == rhs.decimal();
            else return false;
        }
    };

    // Execution result
    struct ExecResult {
        std::vector<RntObj> DS;
        std::vector<RntObj> RS;
        ExecResult(std::vector<RntObj> ds, std::vector<RntObj> rs) : DS(std::move(ds)), RS(std::move(rs)) {}
        bool operator==(const ExecResult &rhs) const {
            return DS == rhs.DS && RS == rhs.RS;
        }
    };

    // Utility to dump stack
    inline void dump_stack(const std::vector<RntObj> &stack) {
        if (stack.empty()) LOG("EMPTY");
        for (const auto &obj : stack) {
            if (obj.type == RntType::YVM_UNDEFINED) {
                LOG("| NULL ");
            } else if (obj.type == RntType::YVM_BOOLEAN) {
                LOG("| %s ", (obj.value.boolean) ? "TRUE" : "FALSE");
            } else if (obj.type == RntType::YVM_INTEGER) {
                LOG("| %d ", obj.value.integer);
            } else if (obj.type == RntType::YVM_DECIMAL) {
                LOG("| %f ", obj.value.decimal);
            }
        }
        LOG("\n");
    }

    // Execute code
    void execute(const Bytecode::Data &code, ExecResult *result = nullptr);

} // YVM::Executer

#endif //YRINVM_EXECUTOR_HPP
