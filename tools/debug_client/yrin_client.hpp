#pragma once

#ifndef YRINVM_YRIN_CLIENT_HPP
#define YRINVM_YRIN_CLIENT_HPP

#include "core/yrin_defs.hpp"

#define CONSOLE_SCREEN_WIDTH 80
#define CONSOLE_SCREEN_HEIGHT 25
#include "console/yrin_win_console.hpp"

namespace Yrin::Network {
    class TcpClient;
} // Network

namespace Yrin::DebugVm {

    class Client {
    private:
        Yrin::Console::ConsoleTool console;
        Yrin::Network::TcpClient& netClient;

    private:
        void setup();

    public:
        explicit Client(int port);

        void run();
    };

} // Yrin::Client

#endif //YRINVM_YRIN_CLIENT_HPP
