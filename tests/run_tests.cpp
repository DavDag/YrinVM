#include "tests.hpp"

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

// Entry point for tests
int main() {
    // Run tests
    for (const auto &test : tests_list) {
        test.run();
    }
    return 0;
}
