#pragma once

#ifndef YRINVM_LOADER_HPP
#define YRINVM_LOADER_HPP

#include "common.hpp"

namespace YVM::Loader {

    const Bytecode::Data &load(const char *filePath);

} // YVM::Loader

#endif //YRINVM_LOADER_HPP
