#include "yrin_client.hpp"
#include "network/yrin_net_client.hpp"

Yrin::DebugVm::Client::Client(int port) : netClient(*(new Yrin::Network::TcpClient(port))) {

}

void Yrin::DebugVm::Client::run() {
    setup();

    std::mutex mutex;
    std::unique_lock lock(mutex);
    netClient.getCV().wait(lock, [&]() { return netClient.canConsoleStart(); });

    // Terminate client
    netClient.close();
}
void Yrin::DebugVm::Client::setup() {
    // ASIO client setup
    netClient.start();

    // Console setup
}
