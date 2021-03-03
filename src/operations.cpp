#include "operations.hpp"

namespace YVM::Operations {

    using namespace Executor;

    // UTILS - POP
    inline RntObj pop_from_stack(std::vector<RntObj> &stack) {
        if (stack.empty()) {
            ERROR_LOG("Trying to pop from empty stack.\n");
            throw Exception::YvmExecutorException(Exception::Errors::POP_FROM_EMPTY_STACK);
        }
        const auto &obj = stack.back();
        stack.pop_back();
        return obj;
    }

    // UTILS - LOAD
    inline RntObj load_from_stack(std::vector<RntObj> &stack, int pos) {
        if (stack.size() <= pos) {
            ERROR_LOG("Index out of bounds: trying to access position %d with size %d.\n", pos, stack.size());
            throw Exception::YvmExecutorException(Exception::Errors::INDEX_OUT_OF_BOUNDS);
        }
        return stack[stack.size() - pos - 1];
    }

    // UTILS - PUSH
    inline void push_into_stack(std::vector<RntObj> &stack, const RntObj &obj) {
        stack.push_back(obj);
    }

    // Zero operation
    void op_zero(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{ZERO}\n");
    }

    // Push 'data' into Runtime Stack
    void op_push(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{PUSH}\n");
        if (instr.data_is_int()) {
            push_into_stack(rntStack, RntObj(instr.data_as_int));
        } else if (instr.data_is_dec()) {
            push_into_stack(rntStack, RntObj(instr.data_as_float));
        } else {
            ERROR_LOG("Invalid operation. PUSH operation accepts 1 INTEGER or 1 DECIMAL.\n");
            throw Exception::YvmExecutorException(Exception::Errors::WRONG_ARGUMENT_TYPE);
        }
    }

    // Pop from Runtime Stack
    void op_pop(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{POP}\n");
        pop_from_stack(rntStack);
    }

    // Pop object from Runtime Stack and save it into Data Stack
    void op_store(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{STORE}\n");
        RntObj obj = pop_from_stack(rntStack);
        push_into_stack(dataStack, obj);
    }

    // Copy object from Data Stack ('data' position) into Runtime Stack
    void op_load(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{LOAD}\n");
        RntObj obj = load_from_stack(dataStack, instr.data_as_int);
        push_into_stack(rntStack, obj);
    }

    // Pop two objects from Runtime Stack and push the sum
    void op_add(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{ADD}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj(obj1.integer() + obj2.integer()));
        } else if (obj1.type == RntType::YVM_DECIMAL && obj2.type == RntType::YVM_DECIMAL) {
            push_into_stack(rntStack, RntObj(obj1.decimal() + obj2.decimal()));
        } else {
            ERROR_LOG("Invalid operation. ADD operation accepts 2 INTEGERS or 2 DECIMALS.\n");
            throw Exception::YvmExecutorException(Exception::Errors::WRONG_ARGUMENT_TYPE);
        }
    }

    // Pop two objects from Runtime Stack and push the subtraction
    void op_sub(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{SUB}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj(obj1.integer() - obj2.integer()));
        } else if (obj1.type == RntType::YVM_DECIMAL && obj2.type == RntType::YVM_DECIMAL) {
            push_into_stack(rntStack, RntObj(obj1.decimal() - obj2.decimal()));
        } else {
            ERROR_LOG("Invalid operation. SUB operation accepts 2 INTEGERS or 2 DECIMALS.\n");
            throw Exception::YvmExecutorException(Exception::Errors::WRONG_ARGUMENT_TYPE);
        }
    }

    // Pop two objects from Runtime Stack and push the sub
    void op_mul(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{SUB}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj(obj1.integer() * obj2.integer()));
        } else if (obj1.type == RntType::YVM_DECIMAL && obj2.type == RntType::YVM_DECIMAL) {
            push_into_stack(rntStack, RntObj(obj1.decimal() * obj2.decimal()));
        } else {
            ERROR_LOG("Invalid operation. MUL operation accepts 2 INTEGERS or 2 DECIMALS.\n");
            throw Exception::YvmExecutorException(Exception::Errors::WRONG_ARGUMENT_TYPE);
        }
    }

    // Pop two objects from Runtime Stack and push the sub
    void op_div(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{SUB}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj(obj1.integer() / obj2.integer()));
        } else if (obj1.type == RntType::YVM_DECIMAL && obj2.type == RntType::YVM_DECIMAL) {
            push_into_stack(rntStack, RntObj(obj1.decimal() / obj2.decimal()));
        } else {
            ERROR_LOG("Invalid operation. DIV operation accepts 2 INTEGERS or 2 DECIMALS.\n");
            throw Exception::YvmExecutorException(Exception::Errors::WRONG_ARGUMENT_TYPE);
        }
        // TODO: Div zero
    }

    // Pop two objects from Runtime Stack and push the sub
    void op_mod(const Bytecode::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{SUB}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj(obj1.integer() % obj2.integer()));
        } else {
            ERROR_LOG("Invalid operation. MOD operation use 2 INTEGERS.\n");
            throw Exception::YvmExecutorException(Exception::Errors::WRONG_ARGUMENT_TYPE);
        }
    }

} // YVM

