CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
PROJECT(YrinVM)

OPTION(EXECUTOR_LOG ON)
OPTION(LOADER_LOG ON)
OPTION(RUN_TESTS OFF)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    IF (EXECUTOR_LOG)
        ADD_DEFINITIONS(-DEXECUTOR_LOG)
    ENDIF (EXECUTOR_LOG)
    IF (LOADER_LOG)
        ADD_DEFINITIONS(-DLOADER_LOG)
    ENDIF (LOADER_LOG)
ENDIF ()

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

SET(SOURCE src/loader.hpp src/common.hpp src/loader.cpp src/executor.hpp src/yvmexception.hpp src/executor.cpp src/operations.hpp src/operations.cpp src/bytecode.hpp)

IF (!RUN_TESTS)
    ADD_EXECUTABLE(YrinVM src/main.cpp ${SOURCE})
ELSE ()
    ADD_EXECUTABLE(YrinVM tests/run_tests.cpp ${SOURCE})
ENDIF ()