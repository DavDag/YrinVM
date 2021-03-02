#pragma once

#ifndef YRINVM_EXECUTOR_HPP
#define YRINVM_EXECUTOR_HPP

#include "common.hpp"

namespace YVM::Executor {

    // Execute code
    void execute(const CODE::Data& code);

} // YVM::Executer

#endif //YRINVM_EXECUTOR_HPP
