#include "executor.hpp"
#include "operations.hpp"

namespace YVM::Executor {

    inline void debug_dump_stack(const std::vector<RntObj> &stack) {
        for (const auto &obj : stack) {
            if (obj.type == RntType::YVM_UNDEFINED) {
                EXECUTOR_DEBUG_LOG("| NULL ");
            } else if (obj.type == RntType::YVM_BOOLEAN) {
                EXECUTOR_DEBUG_LOG("| %s ", (obj.value.boolean) ? "TRUE" : "FALSE");
            } else if (obj.type == RntType::YVM_INTEGER) {
                EXECUTOR_DEBUG_LOG("| %d ", obj.value.integer);
            } else if (obj.type == RntType::YVM_DECIMAL) {
                EXECUTOR_DEBUG_LOG("| %f ", obj.value.decimal);
            }
        }
        EXECUTOR_DEBUG_LOG("\n");
    }

    void execute(const Bytecode::Data &code) {
        // Data Stack
        std::vector<RntObj> dataStack;
        // Runtime Stack
        std::vector<RntObj> rntStack;

        // Operation table
        using op_callback = void (*)(const Bytecode::Instruction &, std::vector<RntObj> &, std::vector<RntObj> &);
        const std::vector<op_callback> op_table = {
                Operations::op_zero, Operations::op_push, Operations::op_pop, Operations::op_store, Operations::op_load,
                Operations::op_add, Operations::op_sub, Operations::op_mul, Operations::op_div, Operations::op_mod
        };

        // Main loop
        for (int i = 0; i < code.instructions.size(); ++i) {
            const Bytecode::Instruction &instr = code.instructions[i];
            EXECUTOR_DEBUG_LOG("%05u: 0x%02X 0x%06X %+9d\n", i, instr.opcode, instr.flags, instr.data_as_int);
            op_table[instr.opcode](instr, dataStack, rntStack);
            EXECUTOR_DEBUG_LOG("RuntimeStack: ");
            debug_dump_stack(rntStack);
            EXECUTOR_DEBUG_LOG("DataStack: ");
            debug_dump_stack(dataStack);
            EXECUTOR_DEBUG_LOG("===============================\n");
        }
    }

} // YVM:Executor
