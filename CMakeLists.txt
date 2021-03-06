CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
PROJECT(YrinVM)

OPTION(EXECUTOR_LOG OFF)
OPTION(LOADER_LOG OFF)
OPTION(RUN_TESTS OFF)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
    IF (EXECUTOR_LOG MATCHES ON)
        ADD_DEFINITIONS(-DEXECUTOR_LOG)
    ENDIF ()
    IF (LOADER_LOG MATCHES ON)
        ADD_DEFINITIONS(-DLOADER_LOG)
    ENDIF ()
ENDIF ()

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

SET(SOURCE src/main.cpp src/yvm.hpp src/common/yrin_defs.hpp src/common/yrin_error.hpp src/common/yrin_code_metadata.hpp src/runtime/yrin_datatypes.hpp src/loader/yrin_loader.cpp src/runtime/yrin_vm.cpp src/runtime/yrin_vm.hpp src/runtime/yrin_operations.cpp src/runtime/yrin_stack.hpp)

IF (RUN_TESTS MATCHES ON)
    ADD_EXECUTABLE(YrinVM tests/run_tests.cpp ${SOURCE})
ELSE ()
    ADD_EXECUTABLE(YrinVM src/main.cpp ${SOURCE})
ENDIF ()