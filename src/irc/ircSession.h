#pragma once

#include "ircMetadata.h"
#include "ircMessageHandler.h"
#include "core/logger.h"
#include <boost/asio.hpp>
#include <memory>
#include <deque>
#include <string>

/*
 *  Represents one IRC connection.
 *  Manages connection state, and implements the IRC protocol (sending NICK/USER, parsing incoming lines, handling PING, etc.).
 */

class SessionManager;
class IrcClient;

static constexpr std::size_t    kMaxReadBuffer = 512;

class IrcSession : public std::enable_shared_from_this<IrcSession> {
    friend SessionManager;
    friend IrcClient;
public:
    IrcSession(const boost::asio::any_io_executor& io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue);

    void connect(const ServerConfig& config);
    void disconnect();
    void join(std::string channel);
    void leave(std::string channel);
    void sendMessage(std::string target, std::string message);

private:
    //Start async connection
    void onResolve(boost::asio::ip::tcp::resolver::results_type endpoints);
    //Start reads and IRC client registration to server
    void onConnect();
    //fire off an async read
    void onRead(std::size_t bytes_transferred);
    void onWrite();
    void sendRaw(std::string message);
    void startRead();
    void startWrite();
private:
    std::shared_ptr<spdlog::logger> m_logger = Logger::get().get_network_logger();
    boost::asio::ip::tcp::resolver  m_resolver;
    boost::asio::ip::tcp::socket    m_socket;
    std::string                     m_read_buffer;
    std::deque<std::string>         m_write_queue;
    IrcMessageHandler               m_irc_message_handler;
    int                             m_server_id = -1;
    std::string                     m_nick;
    std::string                     m_username;
    std::string                     m_real_name;
    SessionState                    m_state = SessionState::Default;
};
