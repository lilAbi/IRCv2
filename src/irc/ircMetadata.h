#pragma once

#include <chrono>
#include <string>
#include <variant>

//server config provided by the UI
struct ServerConfig {
    std::string m_host;
    std::string m_port;
    std::string m_nick;
    std::string m_username;
    int         m_server_id;
};

//IRC Network Related Events
//user connected to server
struct ConnectedEvent {
    int m_server_id;
};
//user disconnected from server and why
struct DisconnectedEvent {
    int         m_server_id;
    std::string m_buffer;
};
//user joined channel
struct ChannelJoinedEvent {
    int         m_server_id;
    std::string m_channel;
};
//user left channel
struct ChannelLeftEvent {
    int         m_server_id;
    std::string m_channel;
};
//user received a message
struct MessageReceivedEvent{
    int         m_server_id;
    std::string m_sender;
    std::string m_target;
    std::string m_message;
    std::chrono::system_clock::time_point m_timestamp;
};

using NetworkEventVariant = std::variant<ConnectedEvent, DisconnectedEvent, ChannelJoinedEvent, ChannelLeftEvent, MessageReceivedEvent>;

enum class SessionState {
    Default = 0,
    Registering,
    Resolving,
    Connecting,
    Connected,
    Disconnecting,
    Disconnected
};