#include <utility>

#include "../src/common.hpp"
#include "../src/loader.hpp"
#include "../src/executor.hpp"

using ExecutionResult = YVM::Executor::ExecResult;

// Run test analyzing VM final state.
// LOG data to help understanding errors.
bool expect_result(const char *filePath, const ExecutionResult &expectedResult) noexcept {
    auto *resultState = new ExecutionResult({}, {});
    try {
        // Load code from file
        const YVM::Bytecode::Data &code = YVM::Loader::load(filePath);
        // Run code
        YVM::Executor::execute(code, resultState);
        // Check if result is equal to the expected result
        if (expectedResult == *resultState) return true;
        // Log error data
        LOG("Results\n");
        LOG("DataStack: ");
        YVM::Executor::dump_stack(resultState->DS);
        LOG("RuntimeStack: ");
        YVM::Executor::dump_stack(resultState->RS);
        LOG("Expected Results\n");
        LOG("DataStack: ");
        YVM::Executor::dump_stack(expectedResult.DS);
        LOG("RuntimeStack: ");
        YVM::Executor::dump_stack(expectedResult.RS);
        return false;
    } catch (YVM::Exception::YvmException &exception) {
        // Log error data
        LOG("VM unexpected exception. Error code %d\n", exception.errorCode);
        return false;
    } catch (std::exception &exception) {
        // Log error data
        LOG("Unexpected exception.\n%s\n", exception.what());
        return false;
    }
}

// Run test expecting and exception to be thrown.
// LOG data to help understanding errors.
bool expect_exception(const char *filePath, int errorCode) noexcept {
    auto *resultState = new ExecutionResult({}, {});
    try {
        // Load code from file
        const YVM::Bytecode::Data &code = YVM::Loader::load(filePath);
        // Run code
        YVM::Executor::execute(code, resultState);
        // Log error data
        LOG("Results\n");
        LOG("DataStack: ");
        YVM::Executor::dump_stack(resultState->DS);
        LOG("RuntimeStack:");
        YVM::Executor::dump_stack(resultState->RS);
        return false;
    } catch (YVM::Exception::YvmException &exception) {
        // Check if result is equal to the expected result
        if (exception.errorCode == errorCode) return true;
        // Log error data
        LOG("Result: %d\n", exception.errorCode);
        LOG("Expected Result: %d\n", errorCode);
        return false;
    } catch (std::exception &exception) {
        // Log error data
        LOG("Unexpected exception.\n%s\n", exception.what());
        return false;
    }
}

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
                // Log test data
                LOG("#%-40.40s FAILED\n", test.filePath);
            }
        }
        // Log session results
        LOG("TEST SESSION RESULT [%d/%d]\n", successCount, tests.size());
        LOG("\n");
    }
};

int main() {
    // Initialize tests
    std::vector<TestSession> tests = {
        TestSession{
            .name = "EXAMPLE 1",
            .tests = {
                SingleTest(".\\tests\\00_helloworld.yrin", ExecutionResult({}, {}))
            }
        }
    };
    // Run tests
    for (const auto &test : tests) test.run();
    return 0;
}
