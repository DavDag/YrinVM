#pragma once

#ifndef YRINVM_YRIN_NET_CLIENT_HPP
#define YRINVM_YRIN_NET_CLIENT_HPP

#include <thread>
#include <condition_variable>
#include <memory>
#include <unordered_set>

#ifndef ASIO_HPP
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE

#include <asio.hpp>

#endif

namespace Yrin::Network {

    class TcpClient {
    private:
        asio::io_context _context;
        std::thread _tContext;
        asio::ip::tcp::socket _socket;
        asio::ip::tcp::endpoint _endpoint;
        std::mutex _mutex;

        // Thread synchronization
        bool _canStart = false;
        std::condition_variable _cVar;

        // Data received
        std::vector<BYTE> _bytestream;

    public:
        explicit TcpClient(int port);

        void start();
        void closeAndJoin();

        [[nodiscard]] std::vector<BYTE> getData() const noexcept { return _bytestream; }

        [[nodiscard]] bool canConsoleStart() const noexcept { return _canStart; }
        [[nodiscard]] bool hasReceivedData() const noexcept { return !_bytestream.empty(); }
        [[nodiscard]] std::condition_variable &getCV() noexcept { return _cVar; }
    };

} // Yrin::Network::Client

#endif //YRINVM_YRIN_NET_CLIENT_HPP
