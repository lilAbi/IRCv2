#pragma once
#include "core/logger.h"
#include <imgui.h>
#include <cfloat>
#include <span>
#include <string_view>

#include "irc/ircClient.h"

constexpr ImVec4 k_sidebar_background{0.10F, 0.14F, 0.19F, 1.0F};
constexpr ImVec4 k_sidebar_text{0.72F, 0.76F, 0.82F, 1.0F};
constexpr ImVec4 k_selected_channel_background{0.17F, 0.23F, 0.30F, 1.0F};

struct UiState {
    bool m_show_join_server_window = false;
};

//Should own the view model
class UI {
public:
    UI(IrcClient& client);
    //draw view model
    void draw();

private:
    void draw_header(std::string_view channel_name, float height);
    void draw_sidebar();
    void draw_pop_up_windows();

    void draw_join_server_window();

private:
    Logger*     m_logger = &Logger::get();
    IrcClient&  m_irc_client;
    UiState     m_state;
};
