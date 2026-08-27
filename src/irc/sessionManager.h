#pragma once

#include "container/threadSafeQueue.h"
#include "ircMetadata.h"
#include "ircSession.h"
#include <boost/asio.hpp>

class SessionManager {
public:
    SessionManager(boost::asio::any_io_executor io_executor, ThreadSafeQueue<NetworkEventVariant>& network_event_queue);

    void connect(ServerConfig config);
    void disconnect(int server_id);
    void join(int server_id, std::string channel);
    void leave(int server_id, std::string channel);
    void sendMessage(int server_id, std::string target, std::string message);

private:
    boost::asio::any_io_executor m_io_executor;
    ThreadSafeQueue<NetworkEventVariant>& m_network_event_queue;
    std::unordered_map<int, std::shared_ptr<IrcSession>> m_sessions;
};
