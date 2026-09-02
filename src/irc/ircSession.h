#pragma once

#include "ircMetadata.h"
#include "core/logger.h"
#include "container/threadSafeQueue.h"
#include <boost/asio.hpp>
#include <memory>
#include <deque>
#include <string>


/*
 *  Represents one IRC connection.
 *  Manages connection state, and implements the IRC protocol (sending NICK/USER, parsing incoming lines, handling PING, etc.).
 */

class IrcSession : public std::enable_shared_from_this<IrcSession> {
public:
    IrcSession(const boost::asio::any_io_executor& io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue);

    void connect(ServerConfig config);
    void disconnect();
    void join(std::string channel);
    void leave(std::string channel);
    void sendMessage(std::string target, std::string message);

private:
    void onResolve();
    void onConnect();
    void onRead();
    void onWrite(std::size_t length, const boost::system::error_code& ec);
    void startRead();
    void startWrite();
private:
    ThreadSafeQueue<NetworkEventVariant>& m_network_event_queue;
    std::shared_ptr<spdlog::logger> m_logger = Logger::get().get_network_logger();
    boost::asio::ip::tcp::resolver  m_resolver;
    boost::asio::ip::tcp::socket    m_socket;
    boost::asio::streambuf          m_read_buffer;
    std::deque<std::string>         m_write_queue;
    int                             m_server_id{};
    std::string                     m_nick;
    std::string                     m_username;
    std::string                     m_real_name;
    SessionState                    m_state{};
};
