#include "yrin_server.hpp"
#include "yvm.hpp"

namespace Yrin::Server {

    TcpConnection::TcpConnection(TcpServer &s, asio::io_context &io_context) :
        _server(s), _socket(io_context), _mutex() {

    }

    std::shared_ptr<TcpConnection> TcpConnection::create(TcpServer &server, asio::io_context &context) {
        return std::shared_ptr<TcpConnection>(new TcpConnection(server, context));
    }

    void TcpConnection::start() {
        // Set connection data
        _clientName = _socket.remote_endpoint().address().to_string() + ":" +
                      std::to_string(_socket.remote_endpoint().port());

        // Instruction string
        static const std::string welcomeStr = "Connected to remove YVM debugger. Available commands are:\r\n"
                                              "-\"run\" (or \"r\") to start the VM execution\r\n"
                                              "-\"add breakpoint\" (or \"ab\") <instruction index> to add a breakpoint\r\n"
                                              "-\"rem breakpoint\" (or \"rb\") <instruction index> to remove a breakpoint\r\n"
                                              "-\"continue\" (or \"c\") to continue execution when the VM is paused\r\n"
                                              "-\"step into\" (or \"si\") to progress by only an instruction\r\n"
                                              "-\"step over\" (or \"so\") to progress \"over\" an instruction (ex. over function calls)\r\n";
        LOG("Sending welcome packet to %s\n", _clientName.c_str());
        sendData(welcomeStr.c_str(), welcomeStr.length() + 1);
    }

    void TcpConnection::sendData(const char *data, size_t length) {
        // ASYNC send
        _socket.async_send(asio::buffer(data, length),
                           [me = shared_from_this()](std::error_code ec, size_t length) {
                               if (!ec) {
                                   // On packet sent
                                   me->waitForResponse();
                               } else {
                                   ERROR_LOG("Error sending client data. Error code = %s\n", ec.message().c_str());

                                   // Close connection
                                   me->close();
                               }
                           });
    }

    void TcpConnection::waitForResponse() {
        // Send 'Welcome' message
        _socket.async_receive(asio::buffer(_readBuffer, 128),
                              [me = shared_from_this()](std::error_code ec, size_t length) {
                                  if (!ec) {
                                      if (me->_readBuffer[0] == '\r' || me->_readBuffer[0] == '\n') {
                                          me->waitForResponse();
                                          return;
                                      }
                                      // On packet received
                                      DEBUG_LOG("Packet Read. Size=%zu, Text=%.*s\n", length, (int) length,
                                                me->_readBuffer);
                                      me->_server.handleMsg(std::string(me->_readBuffer, length));
                                      me->waitForResponse();
                                  } else {
                                      ERROR_LOG("Error sending client data. Error code = %s\n", ec.message().c_str());

                                      // Close connection
                                      me->close();
                                  }
                              });
    }

    void TcpConnection::close() {
        // Lock scope
        std::scoped_lock lock(_mutex);

        // Close socket
        if (_socket.is_open()) {
            LOG("Connection closed.\n");
            _socket.close();
        }
    }

    TcpServer::TcpServer(int port) :
        _acceptor(_context, asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), port)) {

    }

    void TcpServer::run() {
        // Start accepting client
        startAccept();

        // Run on its own thread
        _tContext = std::thread([this]() { _context.run(); });

        // TODO: Safe closing method
    }

    void TcpServer::closeAndJoin() {
        // Close connection
        _connection->close();

        // Join thread context
        _tContext.join();
    }

    void TcpServer::handleMsg(const std::string &msg) {
        /*
         * Available commands:
         *
         *  - run (or \"r\")
         *  - add breakpoint (or \"ab\") <index>
         *  - rem breakpoint (or \"rb\") <index>
         *  - continue (or \"c\")
         *  - step into (or \"si\")
         *  - step over (or \"so\")
         *
         *  // TODO: add -query
         */
        auto CmdRun = [&]() {
            if (_vmCanRun) return;
            DEBUG_LOG("Command \"RUN\" received.\n");
            applyDebugVarsChanges();
            _vmCanRun = true;
        };
        auto CmdAddBreak = [&](int lineIndex) {
            // Check for valid index
            if (lineIndex < 0) return;
            DEBUG_LOG("Command \"ADD BREAKPOINT\" at %d received.\n", lineIndex);
            _breakpointsToRemove.erase(lineIndex);
            _breakpointsToAdd.insert(lineIndex);
        };
        auto CmdRemBreak = [&](int lineIndex) {
            // Check for valid index
            if (lineIndex < 0) return;
            DEBUG_LOG("Command \"REMOVE BREAKPOINT\" at %d received.\n", lineIndex);
            _breakpointsToAdd.erase(lineIndex);
            _breakpointsToRemove.insert(lineIndex);
        };
        auto CmdContinue = [&]() {
            DEBUG_LOG("Command \"CONTINUE\" received.\n");
            _vmCmdReceived = VmDebugCmd::Continue;
        };
        auto CmdStepInto = [&]() {
            DEBUG_LOG("Command \"STEP INTO\" received.\n");
            _vmCmdReceived = VmDebugCmd::StepInto;
        };
        auto CmdStepOver = [&]() {
            DEBUG_LOG("Command \"STEP OVER\" received.\n");
            _vmCmdReceived = VmDebugCmd::StepOver;
        };
        auto ParseIntegerFromString = [](const std::string &string) {
            try {
                int line = std::stoi(string, nullptr, 10);
                if (line < 0) {
                    ERROR_LOG("Line index must be greater than 0\n");
                    return -1;
                }
                return line;
            } catch (std::exception &e) {
                ERROR_LOG("Unable to convert \"%s\" into an integer. Error = %s\n", string.c_str(), e.what());
            }
            // Error parsing integer
            return -1;
        };

        // Reset last command
        _vmCmdReceived = VmDebugCmd::Empty;

        // Error ! At least 1 character is needed
        if (msg == "r" || msg == "run") CmdRun();
        else if (msg == "c" || msg == "continue") CmdContinue();
        else if (msg == "si" || msg == "step into") CmdStepInto();
        else if (msg == "so" || msg == "step over") CmdStepOver();
        else if (msg.starts_with("ab ")) CmdAddBreak(ParseIntegerFromString(msg.substr(3)));
        else if (msg.starts_with("add breakpoint")) CmdAddBreak(ParseIntegerFromString(msg.substr(14)));
        else if (msg.starts_with("rb ")) CmdRemBreak(ParseIntegerFromString(msg.substr(3)));
        else if (msg.starts_with("rem breakpoint")) CmdRemBreak(ParseIntegerFromString(msg.substr(14)));
        else {
            ERROR_LOG("Command \"%s\" is not supported.\n", msg.c_str());
            return;
        }

        // Notify VM
        _cVar.notify_one();
    }

    void TcpServer::applyDebugVarsChanges() {
        // Remove breakpoints
        for (const int &b : _breakpointsToRemove) _breakpoints.erase(b);

        // Add breakpoints
        for (const int &b : _breakpointsToAdd) _breakpoints.insert(b);

        // Clear breakpoint lists
        _breakpointsToAdd.clear();
        _breakpointsToRemove.clear();

        DEBUG_LOG("Breakpoint list: ");
        for (const int& b : _breakpoints)
            DEBUG_LOG("| %d ", b);
        DEBUG_LOG("\n");
    }

    VmDebugCmd TcpServer::getDebugCmd() {
        // Reset command
        auto cmd = _vmCmdReceived;
        _vmCmdReceived = VmDebugCmd::Empty;

        // Update Debug vars
        applyDebugVarsChanges();

        // Return command
        return cmd;
    }

    void TcpServer::startAccept() {
        // Create new connection object
        _connection = TcpConnection::create(*this, _context);

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
                                       ERROR_LOG("Error accepting client. Error code = %s\n", ec.message().c_str());
                                   }
                               });
    }


} // Yrin::Server