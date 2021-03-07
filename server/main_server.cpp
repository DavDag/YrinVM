#include "yvm.hpp"
#include "yrin_server.hpp"

inline void log_yvm_server_usage() noexcept {
    LOG("Usage:\n");
    LOG("\t<path>: [REQUIRED] to run program\n");
    LOG("\t-p <port>: [OPTIONAL] to change server port\n");
}

inline void log_port_error() noexcept {
    LOG("Port number must be greater than 1024.\n");
}

void Yrin::VM::run_server(int port) {
    try {
        // Asio server setup
        Yrin::Server::TcpServer server(port);
        server.run();

        // Main loop
        std::mutex mutex;
        std::unique_lock lock(mutex);
        server.getCV().wait(lock, [&server]() { return server.isClientConnected(); });
        while (!ips.empty()) {
            const BYTE &instruction = code[ips.top()++];
            OpTable[instruction](*this);
        }

        // Close server gracefully
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
