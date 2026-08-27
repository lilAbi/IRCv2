#include "sessionManager.h"

SessionManager::SessionManager(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue)
    : m_io_executor(io_executor), m_network_event_queue(network_event_queue) {
}

void SessionManager::connect(ServerConfig config) {

}

void SessionManager::disconnect(int server_id) {

}

void SessionManager::join(int server_id, std::string channel) {

}

void SessionManager::leave(int server_id, std::string channel) {

}

void SessionManager::sendMessage(int server_id, std::string target, std::string message) {

}
