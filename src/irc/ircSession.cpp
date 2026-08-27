#include "ircSession.h"

IrcSession::IrcSession(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant> &network_event_queue)
    : m_io_executor(io_executor), m_network_event_queue(network_event_queue), m_resolver(io_executor), m_socket(io_executor){
}

void IrcSession::connect(ServerConfig config) {

}

void IrcSession::disconnect() {

}

void IrcSession::join(std::string channel) {

}

void IrcSession::leave(std::string channel) {

}

void IrcSession::sendMessage(std::string target, std::string message) {

}

void IrcSession::onResolve() {

}

void IrcSession::onConnect() {

}

void IrcSession::onRead(std::size_t length, const boost::system::error_code &ec) {

}

void IrcSession::onWrite(std::size_t length, const boost::system::error_code &ec) {

}

void IrcSession::startRead() {

}

void IrcSession::startWrite() {

}
