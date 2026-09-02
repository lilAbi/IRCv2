#include "ircSession.h"

IrcSession::IrcSession(const boost::asio::any_io_executor& io_executor, ThreadSafeQueue<NetworkEventVariant> &network_event_queue)
    : m_resolver(io_executor), m_socket(io_executor), m_network_event_queue(network_event_queue) {
}

void IrcSession::connect(ServerConfig config) {
    m_logger->info("Attempting to resolve an endpoint to host: {}", config.m_host);
    //attempt to resolve and endpoint
    const auto endpoints = m_resolver.resolve(config.m_host, config.m_port);
    if ( !endpoints.empty() ) {
        //grab and use the first endpoint
        const auto endpoint = endpoints.begin()->endpoint();
        m_logger->info("Found and using endpoint: {}", endpoint.address().to_string());
        //attempt to connect to the endpoint
        m_socket.async_connect(endpoint, [this, current_session = this->shared_from_this()] (const boost::system::error_code& error) {
            if (!error) {
                m_logger->info("Connection established");
                //create a buffer to store the incoming session data
                auto buffer = std::make_shared<std::array<char, 4096>>();
                //read from the socket
                current_session->m_socket.async_read_some(
                    boost::asio::buffer(buffer->data(), buffer->size()),
                    [this, buffer = std::move(buffer)](const boost::system::error_code& error, std::size_t bytes_transferred) {
                        if (!error) {
                            m_logger->info("{}", buffer->data());
                        } else {
                            m_logger->error("[read failed] {}", error.message());
                        }
                    }
                );
            } else {
                m_logger->error("[Connection failed] {}", error.message());
            }
        }); //end of async_connect
    }
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
