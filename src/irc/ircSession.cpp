#include "ircSession.h"

IrcSession::IrcSession(const boost::asio::any_io_executor& io_executor, ThreadSafeQueue<NetworkEventVariant> &network_event_queue)
    : m_resolver(io_executor), m_socket(io_executor), m_network_event_queue(network_event_queue) {
}

void IrcSession::connect(ServerConfig config) {
    //update state to Resolving
    m_state = SessionState::Resolving;
    m_logger->info("Attempting to resolve an endpoint to host: {}", config.m_host);
    //resolve the host and get a list of endpoints
    const auto endpoints = m_resolver.resolve(config.m_host, config.m_port);
    if ( !endpoints.empty() ) {
        //use the first endpoint
        const auto endpoint = endpoints.begin()->endpoint();
        m_logger->info("Found and using endpoint: {}", endpoint.address().to_string());
        //update state to Connecting
        m_state = SessionState::Connecting;
        //attempt to connect to the endpoint
        m_socket.async_connect(
            endpoint,
            [this, config = std::move(config), current_session = this->shared_from_this()] (const boost::system::error_code& error) {
                if (!error) {
                    m_logger->info("Connection Established 🟢");
                    current_session->updateServerConfig(config);
                    current_session->onRead();
                } else {
                    m_logger->error("[IrcSession::connect] {}", error.message());
                    current_session->m_state = SessionState::Disconnected;
                }
            }
        ); //end of async_connect
    }
}

void IrcSession::disconnect() {
    //cancel all queued async operations and close the socket
    m_socket.cancel();
    m_socket.close();
    m_state = SessionState::Disconnected;
    m_logger->warn("Session Disconnected");
}

void IrcSession::join(std::string channel) {

}

void IrcSession::leave(std::string channel) {

}

void IrcSession::sendMessage(std::string target, std::string message) {

}

void IrcSession::updateServerConfig(ServerConfig config) {
    m_state     = SessionState::Connected;
    m_server_id = config.m_server_id;
    m_nick      = std::move(config.m_nick);
    m_username  = std::move(config.m_username);
    m_real_name = std::move(config.m_nick);
}

void IrcSession::onResolve() {

}

void IrcSession::onConnect() {

}

void IrcSession::onRead() {
    //todo: find a better way to ready
    //create a char buffer on the heap
    auto buffer = std::make_shared<std::array<char, 1024>>();
    buffer->fill('\0');
    //read from the socket into buffer
    m_socket.async_read_some(
        boost::asio::buffer(buffer->data(), buffer->size()),
        [this, buffer = buffer, current_session = this->shared_from_this()] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                m_logger->info("Response:\n{}", buffer->data());
                //kick off another read from the socket
                current_session->onRead();
            } else {
                m_logger->error("[IrcSession::onRead]: {}", error.message());
                //todo: clean up
                current_session->disconnect();
            }
        }
    ); //end of async_read_some
}

void IrcSession::onWrite(std::size_t length, const boost::system::error_code &ec) {

}

void IrcSession::startRead() {

}

void IrcSession::startWrite() {

}
