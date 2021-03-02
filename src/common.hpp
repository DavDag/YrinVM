#pragma once

#ifndef YRINVM_COMMON_HPP
#define YRINVM_COMMON_HPP

#define LOG(...) fprintf(stdout, __VA_ARGS__)

#define ERROR_LOG(...) fprintf(stderr, __VA_ARGS__)

#ifndef NDEBUG
#define DEBUG_LOG(...) fprintf(stdout, __VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#ifdef LOADER_LOG
#define LOADER_DEBUG_LOG(...) DEBUG_LOG(__VA_ARGS__)
#else
#define LOADER_DEBUG_LOG(...)
#endif

#ifdef EXECUTOR_LOG
#define EXECUTOR_DEBUG_LOG(...) DEBUG_LOG(__VA_ARGS__)
#else
#define EXECUTOR_DEBUG_LOG(...)
#endif

#include <cstdio>
#include <cstdint>
#include <vector>
#include "yvmexception.hpp"
#include "bytecode.hpp"

#endif //YRINVM_COMMON_HPP
