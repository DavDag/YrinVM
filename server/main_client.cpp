#include "yrin_defs.hpp"
#include <cstdlib>

inline void log_yvm_client_usage() noexcept {
    LOG("Usage:\n");
    LOG("\t-p <port>: [OPTIONAL] to change client port\n");
}

inline void log_port_error() noexcept {
    LOG("Port number must be greater than 1024.\n");
}

int main(int argc, char **argv) {
    int port = DEFAULT_SERVER_PORT;

    // Check arguments
    if (argc == 1) {
        // Okay
    } else if (argc == 3) {
        // Check if user has specified a port
        if (argv[1][0] == '-' && argv[1][1] == 'p' && argv[1][2] == '\0') {
            port = atoi(argv[2]);
            // Check if port number is valid
            if (port < 1024) {
                log_port_error();
                return 0;
            }
        } else {
            // Invalid arguments
            log_yvm_client_usage();
            return 0;
        }
    }

    // TODO: asio client setup
    return 0;
}