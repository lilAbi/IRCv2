#pragma once

#include "ircMetadata.h"
#include "container/threadSafeQueue.h"
#include <boost/asio.hpp>
#include <memory>
#include <deque>
#include <string>

class IrcSession : public std::enable_shared_from_this<IrcSession> {
public:
    IrcSession(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue);

    void connect(ServerConfig config);
    void disconnect();
    void join(std::string channel);
    void leave(std::string channel);
    void sendMessage(std::string target, std::string message);

private:
    void onResolve();
    void onConnect();
    void onRead(std::size_t length, const boost::system::error_code& ec);
    void onWrite(std::size_t length, const boost::system::error_code& ec);
    void startRead();
    void startWrite();
private:
    boost::asio::any_io_executor    m_io_executor{};
    ThreadSafeQueue<NetworkEventVariant>& m_network_event_queue;
    boost::asio::ip::tcp::resolver  m_resolver;
    boost::asio::ip::tcp::socket    m_socket;
    boost::asio::streambuf          m_read_buffer{};
    std::deque<std::string>         m_write_queue{};
    int                             m_server_id;
    std::string                     m_nick;
    std::string                     m_username;
    std::string                     m_real_name;
    SessionState                    m_state{};
};
