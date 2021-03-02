#pragma once

#ifndef YRINVM_LOADER_HPP
#define YRINVM_LOADER_HPP

#include "common.hpp"

namespace YVM::Loader {

    // Load code from file (argv[1] contains file path)
    const Bytecode::Data& load(int argc, char **argv);

    // Check if an Instruction is well-formed.
    void check_instruction(const Bytecode::Instruction &i);

} // YVM::Loader

#endif //YRINVM_LOADER_HPP
