#pragma once

#ifndef YRINVM_OPERATIONS_HPP
#define YRINVM_OPERATIONS_HPP

#include "common.hpp"
#include "executor.hpp"

namespace YVM::Operations {

    // Reserved operation
    void op_zero(const Bytecode::Instruction &,
                 std::vector<Executor::RntObj> &dataStack,
                 std::vector<Executor::RntObj> &rntStack);

    void op_push(const Bytecode::Instruction &,
                 std::vector<Executor::RntObj> &dataStack,
                 std::vector<Executor::RntObj> &rntStack);

    void op_pop(const Bytecode::Instruction &,
                std::vector<Executor::RntObj> &dataStack,
                std::vector<Executor::RntObj> &rntStack);

    void op_store(const Bytecode::Instruction &,
                  std::vector<Executor::RntObj> &dataStack,
                  std::vector<Executor::RntObj> &rntStack);

    void op_load(const Bytecode::Instruction &,
                 std::vector<Executor::RntObj> &dataStack,
                 std::vector<Executor::RntObj> &rntStack);

    void op_add(const Bytecode::Instruction &,
                std::vector<Executor::RntObj> &dataStack,
                std::vector<Executor::RntObj> &rntStack);

    void op_sub(const Bytecode::Instruction &,
                std::vector<Executor::RntObj> &dataStack,
                std::vector<Executor::RntObj> &rntStack);

    void op_mul(const Bytecode::Instruction &,
                std::vector<Executor::RntObj> &dataStack,
                std::vector<Executor::RntObj> &rntStack);

    void op_div(const Bytecode::Instruction &,
                std::vector<Executor::RntObj> &dataStack,
                std::vector<Executor::RntObj> &rntStack);

    void op_mod(const Bytecode::Instruction &,
                std::vector<Executor::RntObj> &dataStack,
                std::vector<Executor::RntObj> &rntStack);

} // YVM::Operations

#endif //YRINVM_OPERATIONS_HPP
