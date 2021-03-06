#include "yvm.hpp"

namespace Yrin {

    Memory::StackMemory::ElementType Memory::StackMemory::TypeTable[256] = {0};

    void Memory::StackMemory::init_table() noexcept {
        TypeTable[TypeId::Undefined].size = 0;
        TypeTable[TypeId::Integer].size = sizeof(int);
        TypeTable[TypeId::LongLong].size = sizeof(long long);
        TypeTable[TypeId::Floating].size = sizeof(float);
        TypeTable[TypeId::Double].size = sizeof(double);
        TypeTable[TypeId::Boolean].size = sizeof(bool);
        TypeTable[TypeId::Character].size = sizeof(char);
        TypeTable[TypeId::Pointer].size = sizeof(void*);
    }

} // Yrin

