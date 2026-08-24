#pragma once
#include "core/logger.h"
#include <imgui.h>
#include <array>
#include <cfloat>
#include <functional>
#include <span>
#include <string>
#include <string_view>

struct Channel {
    std::string m_name;
};

struct ChatMessage {
    std::string m_timestamp;
    std::string m_author;
    std::string m_text;
    ImVec4      m_author_color{0.25F, 0.65F, 1.0F, 1.0F};
};

struct Member {
    std::string m_name;
    bool        m_is_operator{false};
    ImVec4      m_color{0.8F, 0.8F, 0.8F, 1.0F};
};

struct ChatUiState {
    int                     m_selected_channel{0};
    std::array<char, 2048>  m_composer{};
    ImGuiTextFilter         m_member_filter;
    std::size_t             m_previous_message_count{0};
    bool                    m_scroll_to_bottom{true};
    bool                    m_focus_composer{false};
};

using SendMessageCallback = std::function<void(std::string)>;

constexpr ImVec4 k_sidebar_background{0.10F, 0.14F, 0.19F, 1.0F};
constexpr ImVec4 k_sidebar_text{0.72F, 0.76F, 0.82F, 1.0F};
constexpr ImVec4 k_selected_channel_background{0.17F, 0.23F, 0.30F, 1.0F};

//Should own the view model
class UI {
public:
    //draw view model
    void draw(
        ChatUiState& state,
        std::span<const Channel> channels,
        std::span<const ChatMessage> messages,
        std::span<const Member> members,
        const SendMessageCallback& on_send
    );
    void draw_header(std::string_view channel_name, float height);
    void draw_sidebar(ChatUiState& state, std::span<const Channel> channels);

private:
    Logger*                     m_logger = &Logger::get();

};
