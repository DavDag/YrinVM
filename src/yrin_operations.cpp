#include "yvm.hpp"

#include <typeinfo>

namespace Yrin {

    // ==============================
    // ||      Vm Operations       ||
    // ==============================

    int op_error(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ERROR}\n");
        return 0;
    }

    int op_reserved(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{RESERVED}\n");
        return 0;
    }

    int op_return(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{RETURN}\n");
        vm.ret();
        return 0;
    }

    template<typename T>
    int op_push(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSH} %s\n", typeid(T).name());
        int ind = 0;
        T i = *reinterpret_cast<T *>(vm.next(ind, sizeof(T)));
        vm.push(i);
        return 0;
    }

    int op_push_b0(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSH} %s\n", typeid(false).name());
        vm.push(false);
        return 0;
    }

    int op_push_b1(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSH} %s\n", typeid(true).name());
        vm.push(true);
        return 0;
    }

    int op_push_s(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSHs}\n");
        return 0;
    }

    int op_pop(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{POP}\n");
        vm.pop();
        return 0;
    }

    int op_load(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{LOAD}\n");
        int ind = 0;
        int i = *reinterpret_cast<int *>(vm.next(ind, 2 * sizeof(BYTE))) & 0x0000ffff;
        vm.load(i);
        return 0;
    }

    int op_store(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{STORE}\n");
        vm.store();
        return 0;
    }

    template<typename T1, typename T2>
    int op_add(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ADD} %s + %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = vm.pop<T1>();
        T2 op2 = vm.pop<T2>();
        vm.push(op1 + op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_sub(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{SUB} %s - %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = vm.pop<T1>();
        T2 op2 = vm.pop<T2>();
        vm.push(op1 - op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_mul(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MUL} %s * %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = vm.pop<T1>();
        T2 op2 = vm.pop<T2>();
        vm.push(op1 * op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_div(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{DIV} %s / %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = vm.pop<T1>();
        T2 op2 = vm.pop<T2>();
        vm.push(op1 / op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_mod(VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MOD} %s %% %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = vm.pop<T1>();
        T2 op2 = vm.pop<T2>();
        vm.push(op1 % op2);
        return 0;
    }

    op_callback VM::OpTable[256] = {op_error};
    void VM::init_table() noexcept {
        auto &t1 = VM::OpTable;
        // Specials
        t1[OP_RESERVED_CODE] = op_reserved;
        t1[OP_RETURN_CODE] = op_return;

        // Push, Pop, Load, Store
        t1[OP_PUSH_I_CODE] = op_push<int>;
        t1[OP_PUSH_L_CODE] = op_push<long long>;
        t1[OP_PUSH_F_CODE] = op_push<float>;
        t1[OP_PUSH_D_CODE] = op_push<double>;
        t1[OP_PUSH_B0_CODE] = op_push_b0;
        t1[OP_PUSH_B1_CODE] = op_push_b1;
        t1[OP_PUSH_C_CODE] = op_push<char>;
        t1[OP_PUSH_S_CODE] = op_push_s;
        t1[OP_POP_CODE] = op_pop;
        t1[OP_LOAD_CODE] = op_load;
        t1[OP_STORE_CODE] = op_store;

        // Arithmetic (Add, Sub, Mul, Div, Mod)
        t1[OP_ADD_I_CODE] = op_add<int, int>;
        t1[OP_ADD_L_CODE] = op_add<long long, long long>;
        t1[OP_ADD_F_CODE] = op_add<float, float>;
        t1[OP_ADD_D_CODE] = op_add<double, double>;
        t1[OP_SUB_I_CODE] = op_sub<int, int>;
        t1[OP_SUB_L_CODE] = op_sub<long long, long long>;
        t1[OP_SUB_F_CODE] = op_sub<float, float>;
        t1[OP_SUB_D_CODE] = op_sub<double, double>;
        t1[OP_MUL_I_CODE] = op_mul<int, int>;
        t1[OP_MUL_L_CODE] = op_mul<long long, long long>;
        t1[OP_MUL_F_CODE] = op_mul<float, float>;
        t1[OP_MUL_D_CODE] = op_mul<double, double>;
        t1[OP_DIV_I_CODE] = op_div<int, int>;
        t1[OP_DIV_F_CODE] = op_div<long long, long long>;
        t1[OP_DIV_L_CODE] = op_div<float, float>;
        t1[OP_DIV_D_CODE] = op_div<double, double>;
        t1[OP_MOD_I_CODE] = op_mod<int, int>;
    }

} // Yrin
