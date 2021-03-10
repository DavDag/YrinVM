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
#include <vector>

using BYTE = unsigned char;

constexpr int DEFAULT_SERVER_PORT = 13131;

constexpr BYTE CURRENT_YVM_VERSION_MAJOR = '0';
constexpr BYTE CURRENT_YVM_VERSION_MINOR = '1';
constexpr BYTE CURRENT_YVM_VERSION_RTYPE = 'b';
constexpr BYTE CURRENT_YVM_VERSION_COUNT = '1';

/*
 * Yrin VM Operation codes
 */

constexpr int OP_RESERVED_CODE = 0;
constexpr int OP_RETURN_CODE = 1;

constexpr int OP_PUSH_I_CODE = 11;
constexpr int OP_PUSH_L_CODE = 12;
constexpr int OP_PUSH_F_CODE = 13;
constexpr int OP_PUSH_D_CODE = 14;
constexpr int OP_PUSH_B0_CODE = 15;
constexpr int OP_PUSH_B1_CODE = 16;
constexpr int OP_PUSH_C_CODE = 17;
constexpr int OP_PUSH_S_CODE = 18;

constexpr int OP_POP_CODE = 20;
constexpr int OP_LOAD_CODE = 21;
constexpr int OP_STORE_CODE = 22;

constexpr int OP_ADD_I_CODE = 30;
constexpr int OP_ADD_L_CODE = 31;
constexpr int OP_ADD_F_CODE = 32;
constexpr int OP_ADD_D_CODE = 33;
constexpr int OP_SUB_I_CODE = 34;
constexpr int OP_SUB_L_CODE = 35;
constexpr int OP_SUB_F_CODE = 36;
constexpr int OP_SUB_D_CODE = 37;
constexpr int OP_MUL_I_CODE = 38;
constexpr int OP_MUL_L_CODE = 39;
constexpr int OP_MUL_F_CODE = 40;
constexpr int OP_MUL_D_CODE = 41;
constexpr int OP_DIV_I_CODE = 42;
constexpr int OP_DIV_F_CODE = 43;
constexpr int OP_DIV_L_CODE = 44;
constexpr int OP_DIV_D_CODE = 45;
constexpr int OP_MOD_I_CODE = 46;

#endif //YRINVM_YRIN_DEFS_HPP
