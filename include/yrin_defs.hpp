#pragma once

#ifndef YRINVM_YRIN_DEFS_HPP
#define YRINVM_YRIN_DEFS_HPP

#define LOG(...) fprintf(stdout, __VA_ARGS__)

#define ERROR_LOG(...) fprintf(stderr, __VA_ARGS__)

#ifdef NDEBUG
#define DEBUG_LOG(...)
#else
#define DEBUG_LOG(...) fprintf(stdout, __VA_ARGS__)
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

using BYTE = unsigned char;

#endif //YRINVM_YRIN_DEFS_HPP
