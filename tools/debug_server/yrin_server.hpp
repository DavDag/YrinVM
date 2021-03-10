#pragma once

#ifndef YRINVM_YRIN_SERVER_HPP
#define YRINVM_YRIN_SERVER_HPP

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

namespace Yrin::Server {

    class TcpServer;

    // TCP Connection
    class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
    private:
        // ASIO
        asio::ip::tcp::socket _socket;
        std::mutex _mutex;

        // Data
        std::string _clientName;
        char _readBuffer[128]{'\0'};

        // VM Debug vars
        TcpServer &_server;

    private:
        // Constructor
        explicit TcpConnection(TcpServer &s, asio::io_context &io_context);

    public:
        // Create a new TcpConnection wrapped into a shared_ptr
        static std::shared_ptr<TcpConnection> create(TcpServer &server, asio::io_context &context);

        // Connection management
        void start();
        void close();
        // [ASYNC]
        void sendData(const char *data, size_t length);
        void waitForResponse();

        // Retrieve socket
        asio::ip::tcp::socket &socket() { return _socket; }
    };

    enum class VmDebugCmd: int {
        Empty = 0, // Reserved for empty command
        Continue = 1,
        StepInto = 2,
        StepOver = 3
    };

    // TCP Server
    class TcpServer {
    private:
        // ASIO
        asio::io_context _context;
        std::thread _tContext;
        asio::ip::tcp::acceptor _acceptor;
        std::shared_ptr<TcpConnection> _connection;

        // Thread Synchronization
        std::condition_variable _cVar;
        bool _vmCanRun = false;
        VmDebugCmd _vmCmdReceived = VmDebugCmd::Empty;

        // VM Debug vars
        std::unordered_set<int> _breakpoints;
        std::unordered_set<int> _breakpointsToAdd;
        std::unordered_set<int> _breakpointsToRemove;

    private:
        // Apply changes to VM debug vars
        void applyDebugVarsChanges();

    public:
        // Constructor
        explicit TcpServer(int port);

        // Server management
        void run();
        void closeAndJoin();
        void handleMsg(const std::string&);

        // Thread synchronization
        std::condition_variable &getCV() { return _cVar; }
        [[nodiscard]] bool canVmRun() const { return _vmCanRun; }
        [[nodiscard]] bool canExecuteInstruction(int index) const { return !_breakpoints.contains(index); }
        [[nodiscard]] bool canVmContinue() const { return _vmCmdReceived != VmDebugCmd::Empty; }
        [[nodiscard]] VmDebugCmd getDebugCmd();

    private:
        // Start accepting client
        void startAccept();
    };

} // Yrin::Server

#endif //YRINVM_YRIN_SERVER_HPP
