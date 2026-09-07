#include "ircSession.h"

IrcSession::IrcSession(const boost::asio::any_io_executor& io_executor, ThreadSafeQueue<NetworkEventVariant> &network_event_queue)
    : m_resolver(io_executor), m_socket(io_executor), m_network_event_queue(network_event_queue) {
}

void IrcSession::connect(ServerConfig config) {
    //update state to Resolving
    m_state = SessionState::Resolving;
    m_server_id = config.m_server_id;
    m_nick      = config.m_nick;
    m_username  = config.m_username;
    m_real_name = config.m_nick;
    m_resolver.async_resolve(
        config.m_host,
        config.m_port,
        [this, current_session = shared_from_this()] (const boost::system::error_code& error, boost::asio::ip::tcp::resolver::results_type results) {
            if (error) {
                m_logger->error("Could not resolve endpoint: {}", error.message());
                return;
            }
            current_session->onResolve(std::move(results));
        }
    );

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

void IrcSession::onResolve(boost::asio::ip::tcp::resolver::results_type endpoints) {
    m_state = SessionState::Connecting;
    boost::asio::async_connect(m_socket, endpoints,
        [this, current_session = shared_from_this()] (const boost::system::error_code& error, const boost::asio::ip::tcp::endpoint& endpoint) {
            if (error) {
                m_logger->error("Could not connect: {}", error.message());
                return;
            }
            m_logger->info("IRC session connected to {}", endpoint.address().to_string());
            current_session->onConnect();
        }
    );
}

void IrcSession::onConnect() {
    m_state = SessionState::Registering;
    startRead();
    sendRaw("CAP LS 302\r\n");
    sendRaw("CAP END\r\n");
    sendRaw("NICK " + m_nick + "\r\n");
    sendRaw("USER " + m_username + " 0 * :" + m_real_name + "\r\n");
}

void IrcSession::onRead() {
    m_logger->info("Response:\n{}", m_read_buffer->data());
    this->startRead();
}

void IrcSession::onWrite(std::size_t length, const boost::system::error_code &ec) {

}

void IrcSession::sendRaw(std::string message) {
    //if queue is empty then there is work in progress
    const auto write_in_progress = !m_write_queue.empty();
    m_write_queue.push_back(std::move(message));
    if (!write_in_progress) {
        startWrite();
    }
}

void IrcSession::startRead() {
    //clear buffer
    m_read_buffer->fill('\0');
    m_socket.async_read_some(
        boost::asio::buffer(m_read_buffer->data(), m_read_buffer->max_size()),
        [this, current_session = this->shared_from_this()] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (error) {
                m_logger->error("Read Error: {}", error.message());
                current_session->disconnect();
                return;
            }
            current_session->onRead();
        }
    );
}

void IrcSession::startWrite() {
    if (m_write_queue.empty()) {
        return;
    }
    m_socket.async_write_some(
        boost::asio::buffer(m_write_queue.front()),
        [this, current_session = this->shared_from_this()] (const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (error) {
                m_logger->error("Write Error: {}", error.message());
                return;
            }
            current_session->m_write_queue.pop_front();
            if (!current_session->m_write_queue.empty()) {
                current_session->startWrite();
            }
        }
    );
}
