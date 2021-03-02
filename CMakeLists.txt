cmake_minimum_required(VERSION 3.17)
project(YrinVM)

OPTION(EXECUTOR_LOG "Executor should print debug log ?" ON)
OPTION(LOADER_LOG "Loader should print debug log ?" ON)

IF(EXECUTOR_LOG)
    ADD_DEFINITIONS(-DEXECUTOR_LOG)
ENDIF(EXECUTOR_LOG)

IF(LOADER_LOG)
    ADD_DEFINITIONS(-DLOADER_LOG)
ENDIF(LOADER_LOG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR})

add_executable(YrinVM src/main.cpp src/loader.hpp src/common.hpp src/loader.cpp src/executor.hpp src/yvmexception.hpp src/executor.cpp)