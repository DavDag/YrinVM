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

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(include)
SET(SOURCE
        src/yrin_loader.cpp
        src/yrin_vm.cpp
        src/yrin_operations.cpp
        src/yrin_stack.cpp)

IF (RUN_TESTS MATCHES ON)
    ADD_EXECUTABLE(YrinVM tests/main_tests.cpp ${SOURCE})
ELSE ()
    ADD_EXECUTABLE(YrinVM src/main.cpp ${SOURCE})
ENDIF ()

INCLUDE_DIRECTORIES(vendor/asio_1_18_1/include)
ADD_EXECUTABLE(YrinVM_server server/yrin_server.cpp server/main_server.cpp ${SOURCE})
ADD_EXECUTABLE(YrinVM_client server/main_client.cpp)