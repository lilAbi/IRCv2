#pragma once

#include "container/threadSafeQueue.h"
#include "core/logger.h"
#include "ircMetadata.h"
#include "ircSession.h"
#include <boost/asio.hpp>

/*
 * Handles creation of new sessions on connect, and dispatches send/join commands to them.
 * Ensures only one active session per server_id .
 */

class SessionManager {
public:
    SessionManager(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue);

    void connect(ServerConfig config);
    void disconnect(int server_id);
    void join(int server_id, std::string channel);
    void leave(int server_id, std::string channel);
    void sendMessage(int server_id, std::string target, std::string message);

private:
    std::shared_ptr<spdlog::logger>         m_logger = Logger::get().get_network_logger();
    ThreadSafeQueue<NetworkEventVariant>&   m_network_event_queue;
    boost::asio::any_io_executor            m_io_executor;
    std::unordered_map<int, std::shared_ptr<IrcSession>> m_sessions;
};
