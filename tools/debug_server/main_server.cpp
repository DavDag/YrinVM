#include "yvm.hpp"
#include "src/yrin_server.hpp"

inline void log_yvm_server_usage() noexcept {
    LOG("Usage:\n");
    LOG("\t<path>: [REQUIRED] to run program\n");
    LOG("\t-p <port>: [OPTIONAL] to change tools port\n");
}

inline void log_port_error() noexcept {
    LOG("Port number must be greater than 1024.\n");
}

void Yrin::VM::run_server(int port) {
    try {
        EXECUTOR_DEBUG_LOG("=================================\n"
                           "||         SERVER & VM         ||\n"
                           "=================================\n");
        // Server setup
        Yrin::Server::TcpServer server(port);
        server.run();

        // Main loop
        std::mutex mutex;
        std::unique_lock lock(mutex); // Must be the only one owning the conditional_variable

        // Execute instruction at index
        auto execInstruction = [&](int &index) {
            const BYTE &instruction = bytestream[code[index]];
            OpTable[instruction](*this);
            ++index;
        };

        int instructionIndexToStop = -1; // -1 To never stop

        // Wait for client to send 'run' command
        server.getCV().wait(lock, [&server]() { return server.canVmRun(); });
        while (!ips.empty()) {
            // Get current instruction index
            int &instructionIndex = ips.top();

            // Check if can execute
            if (server.canExecuteInstruction(instructionIndex) && instructionIndex != instructionIndexToStop) {
                // Execute it
                execInstruction(instructionIndex);
            } else {
                DEBUG_LOG("IP: %d, IP to STOP: %d\n\n", instructionIndex, instructionIndexToStop);

                // Wait for cmd from tools
                server.getCV().wait(lock, [&server]() { return server.canVmContinue(); });

                // Execute command
                switch (server.getDebugCmd()) {
                    // Continue until next breakpoint is hit
                    case Yrin::Server::VmDebugCmd::Continue:
                        DEBUG_LOG("Execute cmd <continue>\n");
                        execInstruction(instructionIndex);
                        instructionIndexToStop = -1;
                        break;

                        // Execute 1 instruction
                    case Yrin::Server::VmDebugCmd::StepInto:
                        DEBUG_LOG("Execute cmd <step into>\n");
                        execInstruction(instructionIndex);
                        instructionIndexToStop = ips.top();
                        break;

                        // Execute 1 instruction (jumping functions)
                    case Yrin::Server::VmDebugCmd::StepOver:
                        DEBUG_LOG("Execute cmd <step over>\n");
                        execInstruction(instructionIndex);
                        instructionIndexToStop = instructionIndex;
                        break;

                        // Check another time for a 'good' command
                    case Yrin::Server::VmDebugCmd::Empty:
                    default:
                        DEBUG_LOG("Execute cmd <empty>\n");
                        break;
                }
            }
        }

        // Close tools gracefully
        server.closeAndJoin();
    } catch (Yrin::Error::YvmException &exception) {
        ERROR_LOG("VM crashed. Error code %d\n", exception.errorCode);
    } catch (std::exception &exception) {
        ERROR_LOG("Unexpected exception thrown.\n%s\n", exception.what());
    }
}

int main(int argc, char **argv) {
    // Check file path and VM configs
    char *filePath;

    // Server settings
    int port = DEFAULT_SERVER_PORT;

    // Check arguments
    if (argc >= 2) {
        // Get file path
        filePath = argv[1];
        if (argc == 4) {
            // Check if user has specified a port
            if (argv[2][0] == '-' && argv[2][1] == 'p' && argv[2][2] == '\0') {
                port = atoi(argv[3]);
                // Check if port number is valid
                if (port < 1024) {
                    log_port_error();
                    return 0;
                }
            } else {
                // Invalid arguments
                log_yvm_server_usage();
                return 0;
            }
        }
    } else {
        // Invalid arguments
        log_yvm_server_usage();
        return 0;
    }

    // Data initialization
    Yrin::Memory::StackMemory::init_table();
    Yrin::VM::init_table();

    // VM
    Yrin::VM vm;
    vm.read(filePath);
    vm.run_server(port);
    return 0;
}
