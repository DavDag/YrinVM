#pragma once

#ifndef YRINVM_YRIN_DATATYPES_HPP
#define YRINVM_YRIN_DATATYPES_HPP

#include "yrin_defs.hpp"

namespace Yrin {

    // Forward declaration
    class VM;

    // Operation callback
    using op_callback = int (*)(Yrin::VM &) noexcept;

} // Yrin

#endif //YRINVM_YRIN_DATATYPES_HPP
