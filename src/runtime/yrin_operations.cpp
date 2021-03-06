#include "yrin_vm.hpp"
#include <typeinfo>

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

    template<typename T>
    int op_push(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSH} %s\n", typeid(T).name());
        T i = *reinterpret_cast<T *>(vm.next(sizeof(T)));
        vm.push(i);
        return 0;
    }

    int op_push_b0(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSH} %s\n", typeid(false).name());
        vm.push(false);
        return 0;
    }

    int op_push_b1(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{PUSH} %s\n", typeid(true).name());
        vm.push(true);
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

    template<typename T1, typename T2>
    int op_add(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{ADD} %s + %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = *reinterpret_cast<T1 *>(vm.pop());
        T2 op2 = *reinterpret_cast<T2 *>(vm.pop());
        vm.push(op1 + op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_sub(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{SUB} %s - %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = *reinterpret_cast<T1 *>(vm.pop());
        T2 op2 = *reinterpret_cast<T2 *>(vm.pop());
        vm.push(op1 - op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_mul(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MUL} %s * %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = *reinterpret_cast<T1 *>(vm.pop());
        T2 op2 = *reinterpret_cast<T2 *>(vm.pop());
        vm.push(op1 * op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_div(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{DIV} %s / %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = *reinterpret_cast<T1 *>(vm.pop());
        T2 op2 = *reinterpret_cast<T2 *>(vm.pop());
        vm.push(op1 / op2);
        return 0;
    }

    template<typename T1, typename T2>
    int op_mod(Yrin::VM &vm) noexcept {
        EXECUTOR_DEBUG_LOG("{MOD} %s % %s\n", typeid(T1).name(), typeid(T2).name());
        T1 op1 = *reinterpret_cast<T1 *>(vm.pop());
        T2 op2 = *reinterpret_cast<T2 *>(vm.pop());
        vm.push(op1 % op2);
        return 0;
    }

    op_callback Yrin::VM::OpTable[256] = {op_error};
    void Yrin::VM::init_table() noexcept {
        auto &t = Yrin::VM::OpTable;
        // Specials
        t[0] = op_reserved;
        t[1] = op_return;

        // Push, Pop, Load, Store
        t[11] = op_push<int>;
        t[12] = op_push<long long>;
        t[13] = op_push<float>;
        t[14] = op_push<double>;
        t[15] = op_push_b0;
        t[16] = op_push_b1;
        t[17] = op_push<char>;
        t[18] = op_push_s;
        t[20] = op_pop;
        t[21] = op_load;
        t[22] = op_store;

        // Arithmetic (Add, Sub, Mul, Div, Mod)
        t[30] = op_add<int, int>;
        t[31] = op_add<long long, long long>;
        t[32] = op_add<float, float>;
        t[33] = op_add<double, double>;
        t[34] = op_sub<int, int>;
        t[35] = op_sub<long long, long long>;
        t[36] = op_sub<float, float>;
        t[37] = op_sub<double, double>;
        t[38] = op_mul<int, int>;
        t[39] = op_mul<long long, long long>;
        t[40] = op_mul<float, float>;
        t[41] = op_mul<double, double>;
        t[42] = op_div<int, int>;
        t[43] = op_div<long long, long long>;
        t[44] = op_div<float, float>;
        t[45] = op_div<double, double>;
        t[46] = op_mod<int, int>;
    }

} // Yrin
