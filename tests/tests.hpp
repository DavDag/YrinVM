#pragma once

#ifndef YRINVM_TESTS_HPP
#define YRINVM_TESTS_HPP

#include "../src/common.hpp"
#include "../src/loader.hpp"
#include "../src/executor.hpp"

using ExecutionResult = YVM::Executor::ExecResult;
namespace ExecutionErrors = YVM::Exception::Errors;

// Run test analyzing VM final state.
// LOG data to help understanding errors.
bool expect_result(const char *filePath, const ExecutionResult &expectedResult) noexcept;

// Run test expecting and exception to be thrown.
// LOG data to help understanding errors.
bool expect_exception(const char *filePath, int errorCode) noexcept;

// Struct for testing purpose.
// + filePath contains path to code to test.
// + errorCode / expectedResult is the data expected from running test.
struct SingleTest {
    const char *filePath;
    int errorCode;
    ExecutionResult expectedResult;
    // Utility constructor to reduce code for tests
    SingleTest(const char *filePath, int errorCode) : filePath(filePath), errorCode(errorCode),
                                                      expectedResult({}, {}) {}
    // Utility constructor to reduce code for tests
    SingleTest(const char *filePath, ExecutionResult expectedResult) : filePath(filePath), errorCode(-1),
                                                                       expectedResult(std::move(expectedResult)) {}
    // Run test shortcut that choose correct function
    [[nodiscard]] bool run() const noexcept {
        return (errorCode != -1) ? expect_exception(filePath, errorCode) : expect_result(filePath, expectedResult);
    }
};

// Struct for testing purpose.
// + name contains session name
// + tests contains test list
struct TestSession {
    const char *name;
    const std::vector<SingleTest> tests;
    // Run and log entire test session with results
    void run() const noexcept {
        // Log session name
        LOG(">>> %s\n", name);
        int successCount = 0;
        for (const auto &test : tests) {
            if (test.run()) ++successCount;
            else {
                // Log test failed message
                LOG("#%-40.40s FAILED\n", test.filePath);
            }
        }
        // Log session results
        LOG("TEST SESSION RESULT [%d/%d]\n", successCount, tests.size());
        LOG("\n");
    }
};

//
// TO DEFINE USING PYTHON SCRIPT TO GENERATE CODE AND TEST FILES
//
extern std::vector<TestSession> tests_list;

#endif //YRINVM_TESTS_HPP
