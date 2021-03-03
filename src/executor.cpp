#include "executor.hpp"
#include "operations.hpp"

namespace YVM::Executor {

    void execute(const Bytecode::Data &code, ExecResult *result) {
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
#ifdef EXECUTOR_LOG
            dump_stack(rntStack);
#endif
            EXECUTOR_DEBUG_LOG("DataStack: ");
#ifdef EXECUTOR_LOG
            dump_stack(dataStack);
#endif
            EXECUTOR_DEBUG_LOG("===============================\n");
        }

        // Save result if requested
        if (result != nullptr) {
            result->DS = std::vector<RntObj>(dataStack);
            result->RS = std::vector<RntObj>(rntStack);
        }
    }
} // YVM:Executor
