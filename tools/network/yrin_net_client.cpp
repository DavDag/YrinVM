#include "network/yrin_net_client.hpp"
#include "core/yrin_defs.hpp"

Yrin::Network::TcpClient::TcpClient(int port) : _socket(_context),
                                                _endpoint(asio::ip::make_address_v4("127.0.0.1"), port) {

}

void Yrin::Network::TcpClient::start() {
    // Try connect to server
    _socket.async_connect(_endpoint,
                          [&](std::error_code ec) {
                              if (!ec) {
                                  // Connected !
                                  DEBUG_LOG("Connected to %s:%d\n", _endpoint.address().to_string().c_str(),
                                            _endpoint.port());
                                  _canStart = true;
                              } else {
                                  close();
                              }
                          });

    // Launch client on its own thread
    _tContext = std::thread([&] { _context.run(); });
}

void Yrin::Network::TcpClient::close() {
    _context.stop();
}
