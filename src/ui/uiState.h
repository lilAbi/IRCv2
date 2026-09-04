#pragma once

#include <string>

struct JoinServerForm {
    std::string m_host = "192.168.50.226";
    std::string m_port = "6667";
    std::string m_nickname;
    std::string m_username;
    std::string m_real_name;
    std::string m_validation_error;
};

struct UiState {
    // Current view
    int         m_selected_server_id = -1;
    std::string m_selected_channel;
    // Composer
    std::string m_message_input;
    //pop-up
    bool            m_show_join_server_window = false;
    JoinServerForm  m_join_server;
};