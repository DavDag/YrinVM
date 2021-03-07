#pragma once

#ifndef YRINVM_YRIN_SERVER_HPP
#define YRINVM_YRIN_SERVER_HPP

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE

#include <asio.hpp>

namespace Yrin::Server {

    // TCP Connection
    class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
    private:
        // Tcp Socket
        asio::ip::tcp::socket _socket;

        // Mutex
        std::mutex _mutex;

        bool _isActive;

        // Constructor
        explicit TcpConnection(asio::io_context &io_context) : _socket(io_context), _mutex(), _isActive(false) {}

    public:
        // Create a new TcpConnection wrapped into a shared_ptr
        static std::shared_ptr<TcpConnection> create(asio::io_context &context) {
            return std::shared_ptr<TcpConnection>(new TcpConnection(context));
        }

        // Returns socket
        asio::ip::tcp::socket &socket() { return _socket; }

        // On connection started
        void start() {
            _isActive = true;
            static const std::string welcomeStr = "Welcome";

            LOG("Sending welcome packet to %s:%d\n",
                _socket.remote_endpoint().address().to_string().c_str(), _socket.remote_endpoint().port());
            // Send 'Welcome' message
            _socket.async_send(asio::buffer(welcomeStr.c_str(), welcomeStr.length() + 1),
                               [me = shared_from_this()](std::error_code ec, size_t length) {
                                   if (!ec) {
                                       // On packet sent
                                       LOG("Welcome Packet Send !\n");
                                   } else {
                                       LOG("Error sending client data. Error code = %s\n", ec.message().c_str());

                                       // Close connection
                                       me->close();
                                   }
                               });
        }

        void close() {
            // Lock scope
            std::scoped_lock lock(_mutex);

            // Close socket
            if (_socket.is_open()) _socket.close();
            LOG("Connection closed.\n");
        }

        bool isActive() {
            return _isActive;
        }
    };

    // TCP Server
    class TcpServer {
    private:
        // ASIO context and Thread context
        asio::io_context _context;
        std::thread _tContext;

        // TCP acceptor
        asio::ip::tcp::acceptor _acceptor;

        // Connection
        std::shared_ptr<TcpConnection> _connection;

        // Condition variables
        std::condition_variable _cVar;

    public:
        // Constructor
        explicit TcpServer(int port) :
            _acceptor(_context, asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), port)) {}

        // Run server
        void run() {
            // Start accepting client
            startAccept();

            // Run on its own thread
            _tContext = std::thread([this]() { _context.run(); });
        }

        // Close connection and join thread to terminate server correctly
        void closeAndJoin() {
            // Close connection
            _connection->close();
            _context.stop();

            // Join thread context
            _tContext.join();
        }

        std::condition_variable& getCV() {
            return _cVar;
        }

        bool isClientConnected() {
            return _connection != nullptr && _connection->isActive();
        }

    private:
        // Start accepting client
        void startAccept() {
            // Create new connection object
            _connection = TcpConnection::create(_context);

            LOG("Waiting for client to connect on %s:%d ...\n",
                _acceptor.local_endpoint().address().to_string().c_str(), _acceptor.local_endpoint().port());
            // Wait asynchronously for client
            _acceptor.async_accept(_connection->socket(),
                                   [=](std::error_code ec) {
                                       // Check for error
                                       if (!ec) {
                                           LOG("Client accepted.\n");
                                           _connection->start();
                                           _cVar.notify_one();
                                       } else {
                                           // TODO: May try to accept another request ?
                                           LOG("Error accepting client. Error code = %s\n", ec.message().c_str());
                                       }
                                   });
        }
    };

} // Yrin::Server

#endif //YRINVM_YRIN_SERVER_HPP
