#include "network/yrin_net_client.hpp"
#include "core/yrin_defs.hpp"

Yrin::Network::TcpClient::TcpClient(int port) : _socket(_context),
                                                _endpoint(asio::ip::make_address_v4("127.0.0.1"), port) {

}

void Yrin::Network::TcpClient::start() {
    DEBUG_LOG("Trying to connect to %s:%d\n", _endpoint.address().to_string().c_str(), _endpoint.port());

    // Try connect to server
    _socket.async_connect(_endpoint,
                          [&](std::error_code ec) {
                              if (!ec) {
                                  // Connected !
                                  LOG("Connected to %s:%d\n",
                                      _socket.remote_endpoint().address().to_string().c_str(),
                                      _socket.remote_endpoint().port());
                                  _canStart = true;
                                  _cVar.notify_one();
                              } else {
                                  // TODO: May try another time ?
                                  ERROR_LOG("Unable to connect\n");
                                  closeAndJoin();
                              }
                          });

    // Launch client on its own thread
    _tContext = std::thread([&] { _context.run(); });
}

void Yrin::Network::TcpClient::closeAndJoin() {
    std::scoped_lock lock(_mutex);

    if (_socket.is_open()) {
        LOG("Connection closed.\n");
        _socket.close();
    }

    _tContext.join();
}
