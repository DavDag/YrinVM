#include "yrin_client.hpp"
#include "network/yrin_net_client.hpp"

Yrin::DebugVm::Client::Client(int port) : netClient(*(new Yrin::Network::TcpClient(port))) {

}

void Yrin::DebugVm::Client::run() {
    setup();

    // Wait for client
    std::mutex mutex;
    std::unique_lock lock(mutex);
    netClient.getCV().wait(lock, [&]() { return netClient.canConsoleStart(); });

    // Retrieve data and run console
    netClient.getCV().wait(lock, [&]() { return netClient.hasReceivedData(); });
    const std::vector<BYTE>& bytestream = netClient.getData();

    // Terminate client
    close();
}

void Yrin::DebugVm::Client::setup() {
    // ASIO client setup
    netClient.start();

    // Console setup
}

void Yrin::DebugVm::Client::close() {
    netClient.closeAndJoin();
}
