#pragma once
#include "irc/ircMetadata.h"
#include <chrono>
#include <string>
#include <unordered_map>

struct ChatMessage {
    std::string m_sender;
    std::string m_text;
    std::chrono::system_clock::time_point m_timestamp;
};

struct ChannelState {
    std::string m_name;
    std::vector<ChatMessage> m_messages;
    std::vector<std::string> m_users;
};

struct ServerState {
    int m_server_id;
    std::string m_name;
    SessionState m_session_state;
    std::unordered_map<std::string, ChannelState> m_channels;
};

class IrcViewModel {
public:
    std::unordered_map<int, ServerState> m_channels;
};

