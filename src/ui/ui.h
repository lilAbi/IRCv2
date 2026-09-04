#pragma once
#include "uiState.h"
#include "core/logger.h"
#include "irc/ircClient.h"
#include "irc/ircViewModel.h"
#include <imgui.h>

//Should own the view model
class UI {
public:
    explicit UI(IrcClient& client, IrcViewModel& view_model);
    //draw view model
    void draw();
private:
    //main layout
    void draw_sidebar();
    void draw_chat_panel();
    void draw_member_list();
    void draw_member_list(const ChannelState* channel);
    void draw_header(const ServerState& server, const ChannelState& channel, float height);
    void draw_message_history(const ChannelState& channel);
    void draw_composer(const ServerState& server, const ChannelState& channel, float height);
    void draw_empty_chat();
    //helpers
    const ServerState* selected_server() const;
    const ChannelState* selected_channel() const;
    //pop ups
    void draw_pop_up_windows();
    void draw_join_server_window();
private:
    Logger*         m_logger = &Logger::get();
    IrcClient&      m_irc_client;
    IrcViewModel&   m_irc_view_model;
    UiState         m_state;
};
