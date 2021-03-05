#include "yrin_vm.hpp"

namespace Yrin {

    int op_reserved(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{RESERVED}\n");
        return 0;
    }

    int op_error(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ERROR}\n");
        return 0;
    }

    int op_return(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{RETURN}\n");
        vm.ret();
        return 0;
    }

    int op_push_i(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHi}\n");
        int i = *reinterpret_cast<int *>(vm.next(sizeof(int)));
        vm.push(i);
        return 0;
    }

    int op_push_f(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHf}\n");
        float f = *reinterpret_cast<float *>(vm.next(sizeof(float)));
        vm.push(f);
        return 0;
    }

    int op_push_d(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHd}\n");
        double d = *reinterpret_cast<double *>(vm.next(sizeof(double)));
        vm.push(d);
        return 0;
    }

    int op_push_b0(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHb0}\n");
        vm.push(false);
        return 0;
    }

    int op_push_b1(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHb1}\n");
        vm.push(false);
        return 0;
    }

    int op_push_c(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHc}\n");
        char c = *reinterpret_cast<char *>(vm.next(sizeof(char)));
        vm.push(c);
        return 0;
    }

    int op_push_s(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHs}\n");
        return 0;
    }

    int op_pop(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{POP}\n");
        vm.pop();
        return 0;
    }

    int op_load(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{LOAD}\n");
        int i = *reinterpret_cast<int *>(vm.next(2 * sizeof(BYTE))) & 0x0000ffff;
        vm.load(i);
        return 0;
    }

    int op_store(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{STORE}\n");
        vm.store();
        return 0;
    }

    int op_add_i(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ADDi}\n");
        return 0;
    }

    int op_add_f(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ADDf}\n");
        float f1 = *reinterpret_cast<float*>(vm.pop());
        float f2 = *reinterpret_cast<float*>(vm.pop());
        vm.push(f1 + f2);
        return 0;
    }

    int op_add_d(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ADDd}\n");
        return 0;
    }

    int op_sub_i(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{SUBi}\n");
        int i1 = *reinterpret_cast<int*>(vm.pop());
        int i2 = *reinterpret_cast<int*>(vm.pop());
        vm.push(i1 + i2);
        return 0;
    }

    int op_sub_f(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{SUBf}\n");
        return 0;
    }

    int op_sub_d(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{SUBd}\n");
        return 0;
    }

    int op_mul_i(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MULi}\n");
        return 0;
    }

    int op_mul_f(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MULf}\n");
        return 0;
    }

    int op_mul_d(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MULd}\n");
        return 0;
    }

    int op_div_i(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{DIVi}\n");
        return 0;
    }

    int op_div_f(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{DIVf}\n");
        return 0;
    }

    int op_div_d(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{DIVd}\n");
        return 0;
    }

    int op_mod_i(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MODi}\n");
        return 0;
    }

    op_callback Yrin::VM::OpTable[256] = {op_error};
    void Yrin::VM::init_table() noexcept {
        auto &t = Yrin::VM::OpTable;
        // Specials
        t[0] = op_reserved;
        t[1] = op_return;
        // Push, Pop, Load, Store
        t[11] = op_push_i;
        t[12] = op_push_f;
        t[13] = op_push_d;
        t[14] = op_push_b0;
        t[15] = op_push_b1;
        t[16] = op_push_c;
        t[17] = op_push_s;
        t[20] = op_pop;
        t[21] = op_load;
        t[22] = op_store;
        // Arithmetic (Add, Sub, Mul, Div, Mod)
        t[30] = op_add_i;
        t[31] = op_add_f;
        t[32] = op_add_d;
        t[33] = op_sub_i;
        t[34] = op_sub_f;
        t[35] = op_sub_d;
        t[36] = op_mul_i;
        t[37] = op_mul_f;
        t[38] = op_mul_d;
        t[39] = op_div_i;
        t[40] = op_div_f;
        t[41] = op_div_d;
        t[42] = op_mod_i;
    }

} // Yrin
