#include "executor.hpp"

namespace YVM::Executor {

    // Runtime object type
    enum RntType : int {
        YVM_UNDEFINED = 0x0,
        YVM_BOOLEAN = 0x1,
        YVM_INTEGER = 0x2,
        YVM_DECIMAL = 0x3
    };

    // Runtime object representation
    struct RntObj {
        const RntType type = RntType::YVM_UNDEFINED;
        const union {
            int integer;
            bool boolean;
            float decimal;
        };
    };

    inline void debug_dump_stack(const std::vector<RntObj> &stack) {
        for (const auto &obj : stack) {
            if (obj.type == RntType::YVM_UNDEFINED) {
                EXECUTOR_DEBUG_LOG("| NULL ");
            } else if (obj.type == RntType::YVM_BOOLEAN) {
                EXECUTOR_DEBUG_LOG("| %s ", (obj.boolean) ? "TRUE" : "FALSE");
            } else if (obj.type == RntType::YVM_INTEGER) {
                EXECUTOR_DEBUG_LOG("| %d ", obj.integer);
            } else if (obj.type == RntType::YVM_DECIMAL) {
                EXECUTOR_DEBUG_LOG("| %f ", obj.decimal);
            }
        }
        EXECUTOR_DEBUG_LOG("\n");
    }

    // YVM operations
    void op_zero(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);
    void op_push(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);
    void op_pop(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);
    void op_store(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);
    void op_load(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);
    void op_add(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);
    void op_sub(const YVM::CODE::Instruction &, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack);

    void execute(const CODE::Data &code) {
        // Data Stack
        std::vector<RntObj> dataStack;
        // Runtime Stack
        std::vector<RntObj> rntStack;

        // Operation table
        using operation_callback = void (*)(const YVM::CODE::Instruction &, std::vector<RntObj> &,
                                            std::vector<RntObj> &);
        const std::vector<operation_callback> op_table = {
                op_zero, op_push, op_pop, op_store, op_load, op_add, op_sub
        };

        // Main loop
        for (int i = 0; i < code.instructions.size(); ++i) {
            const CODE::Instruction &instruction = code.instructions[i];
            EXECUTOR_DEBUG_LOG("%05u: 0x%02X 0x%06X %+9d\n", i, instruction.opcode, instruction.flags,
                               instruction.data_as_int);
            op_table[instruction.opcode](instruction, dataStack, rntStack);
            EXECUTOR_DEBUG_LOG("RuntimeStack: ");
            debug_dump_stack(rntStack);
            EXECUTOR_DEBUG_LOG("DataStack: ");
            debug_dump_stack(dataStack);
            EXECUTOR_DEBUG_LOG("===============================\n");
        }
    }

    // Shared
    inline RntObj pop_from_stack(std::vector<RntObj> &stack) {
        if (stack.empty()) {
            ERROR_LOG("Trying to pop from empty stack.\n");
            throw Exception::YvmExecutorException(Exception::ERRORS::POP_FROM_EMPTY_STACK);
        }
        const auto &obj = stack.back();
        stack.pop_back();
        return obj;
    }

    inline RntObj load_from_stack(std::vector<RntObj> &stack, int pos) {
        if (stack.size() <= pos) {
            ERROR_LOG("Index out of bounds: trying to access position %d with size %d.\n", pos, stack.size());
            throw Exception::YvmExecutorException(Exception::ERRORS::INDEX_OUT_OF_BOUNDS);
        }
        return stack[stack.size() - pos - 1];
    }

    inline void push_into_stack(std::vector<RntObj> &stack, const RntObj &obj) {
        stack.push_back(obj);
    }

    // Zero operation
    void op_zero(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{ZERO}\n");
    }

    // Push 'data' into Runtime Stack
    void op_push(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{PUSH}\n");
        if (instr.data_is_int()) {
            push_into_stack(rntStack, RntObj{.type = RntType::YVM_INTEGER, .integer = instr.data_as_int});
        } else if (instr.data_is_dec()) {
            push_into_stack(rntStack, RntObj{.type = RntType::YVM_DECIMAL, .decimal = instr.data_as_float});
        }
    }

    // Pop from Runtime Stack
    void op_pop(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{POP}\n");
        pop_from_stack(rntStack);
    }

    // Pop object from Runtime Stack and save it into Data Stack
    void op_store(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{STORE}\n");
        RntObj obj = pop_from_stack(rntStack);
        push_into_stack(dataStack, obj);
    }

    // Copy object from Data Stack ('data' position) into Runtime Stack
    void op_load(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{LOAD}\n");
        RntObj obj = load_from_stack(dataStack, instr.data_as_int);
        push_into_stack(rntStack, obj);
    }

    // Pop two objects from Runtime Stack and push the sum
    void op_add(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{ADD}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj{.type = RntType::YVM_INTEGER, .integer = obj1.integer + obj2.integer});
        } else if (obj1.type == RntType::YVM_DECIMAL && obj2.type == RntType::YVM_DECIMAL) {
            push_into_stack(rntStack, RntObj{.type = RntType::YVM_DECIMAL, .decimal = obj1.decimal + obj2.decimal});
        }
    }

    // Pop two objects from Runtime Stack and push the sub
    void op_sub(const YVM::CODE::Instruction &instr, std::vector<RntObj> &dataStack, std::vector<RntObj> &rntStack) {
        EXECUTOR_DEBUG_LOG("{SUB}\n");
        RntObj obj1 = pop_from_stack(rntStack);
        RntObj obj2 = pop_from_stack(rntStack);
        if (obj1.type == RntType::YVM_INTEGER && obj2.type == RntType::YVM_INTEGER) {
            push_into_stack(rntStack, RntObj{.type = RntType::YVM_INTEGER, .integer = obj1.integer - obj2.integer});
        } else if (obj1.type == RntType::YVM_DECIMAL && obj2.type == RntType::YVM_DECIMAL) {
            push_into_stack(rntStack, RntObj{.type = RntType::YVM_DECIMAL, .decimal = obj1.decimal - obj2.decimal});
        }
    }


} // YVM:Executor
