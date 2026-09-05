#include "ui.h"
#include "imgui_stdlib.h"

UI::UI(IrcClient &client, IrcViewModel& view_model) : m_irc_client(client), m_irc_view_model(view_model) {}

void UI::draw() {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    constexpr ImGuiWindowFlags root_flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    //These styles are captured when the root window begins.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

    if ( ImGui::Begin("##irc-client-root",nullptr, root_flags) ) {
        ImGui::PopStyleVar(3);

        this->draw_pop_up_windows();

        constexpr ImGuiTableFlags layout_flags =
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_BordersInnerV |
            ImGuiTableFlags_SizingStretchProp;

        //root ui layout
        if (const ImVec2 available = ImGui::GetContentRegionAvail(); ImGui::BeginTable("##main-layout", 3, layout_flags, available) ) {
            //left side bar
            ImGui::TableSetupColumn("Channels", ImGuiTableColumnFlags_WidthFixed,210.0F);
            //center
            ImGui::TableSetupColumn("Chat", ImGuiTableColumnFlags_WidthStretch);
            //right side bar
            ImGui::TableSetupColumn("Users", ImGuiTableColumnFlags_WidthFixed, 220.0F);
            //set the size of the next row
            ImGui::TableNextRow(ImGuiTableRowFlags_None, available.y );
            //Draw Left Sidebar
            ImGui::TableSetColumnIndex(0);
            if ( ImGui::BeginChild("##channel-sidebar", ImVec2{0.0F, 0.0F}) ) {
                draw_sidebar();
            }
            ImGui::EndChild();
            //Chat
            ImGui::TableSetColumnIndex(1);
            if ( ImGui::BeginChild("##chat-workspace", ImVec2{0.0F, 0.0F}, ImGuiChildFlags_None,ImGuiWindowFlags_NoScrollbar) ) {
                draw_chat_panel();
            }
            ImGui::EndChild();
            //members
            ImGui::TableSetColumnIndex(2);
            if ( ImGui::BeginChild("##member-sidebar", ImVec2{0.0F, 0.0F}) ) {
                draw_member_list(selected_channel());
            }
            ImGui::EndChild();
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void UI::draw_sidebar() {
    ImGui::TextUnformatted("Channels:");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if (m_irc_view_model.m_servers.empty()) {
        ImGui::TextDisabled("Not connected");
    }
    for (const auto& [server_id, server_state] : m_irc_view_model.m_servers) {
        ImGui::PushID(server_id);
        //server heading
        const std::string server_name = server_state.m_name.empty() ? "Server " + std::to_string(server_id) : server_state.m_name;
        ImGui::TextDisabled("%s", server_name.c_str());
        //status view
        const bool status_selected = m_state.m_selected_server_id == server_id && m_state.m_selected_channel.empty();
        if (ImGui::Selectable("  Status", status_selected)) {
            m_state.m_selected_server_id = server_id;
            m_state.m_selected_channel.clear();
        }
        //channels
        for (const auto& [channel_name, channel] : server_state.m_channels) {
            ImGui::PushID(channel_name.c_str());
            const bool selected = m_state.m_selected_server_id == server_id && m_state.m_selected_channel == channel_name;
            std::string label = "#  " + channel.m_name;
            if ( ImGui::Selectable(label.c_str(), selected) ) {
                m_state.m_selected_server_id = server_id;
                m_state.m_selected_channel = channel_name;
            }
            ImGui::Spacing();
            ImGui::PopID();
        }
        ImGui::Spacing();
        ImGui::PopID();
    }
    const float button_height   = ImGui::GetFrameHeight();
    const float desired_y       = ImGui::GetWindowHeight() - button_height - ImGui::GetStyle().WindowPadding.y;
    if ( ImGui::GetCursorPosY() < desired_y ) ImGui::SetCursorPosY(desired_y);
    if ( ImGui::Button("+ Connect", ImVec2{-FLT_MIN, 0.0F}) ) {
        m_state.m_show_join_server_window = true;
    }
}

void UI::draw_chat_panel() {
    const ServerState*  server = selected_server();
    const ChannelState* channel = selected_channel();
    if ( !server || !channel ) {
        draw_empty_chat();
        return;
    }
    constexpr float header_height = 52.0F;
    constexpr float composer_height = 54.0F;
    draw_header(*server, *channel, header_height);
    //everything remaining except composer
    const float history_height = std::max(1.0F, ImGui::GetContentRegionAvail().y - composer_height - ImGui::GetStyle().ItemSpacing.y);
    if ( ImGui::BeginChild("##message-history", ImVec2{0.0F, history_height}, ImGuiChildFlags_None, ImGuiWindowFlags_AlwaysVerticalScrollbar) ) {
        draw_message_history(*channel);
    }
    ImGui::EndChild();
    draw_composer(*server, *channel, composer_height);
}

void UI::draw_member_list(const ChannelState* channel) {
    ImGui::TextUnformatted("Users:");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if ( !channel ) {
        ImGui::TextDisabled("No channel selected");
        return;
    }
    for (const std::string& user : channel->m_users) {
        ImGui::PushID(user.c_str());
        char initial[2] { user.empty() ? '?' : static_cast<char>( std::toupper(static_cast<unsigned char>(user.front())) ), '\0' };
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0F);
        ImGui::Button(initial, ImVec2{32.0F, 32.0F});
        ImGui::PopStyleVar();
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(user.c_str());
        ImGui::PopID();
    }
}

void UI::draw_header(const ServerState& server, const ChannelState& channel, float height) {
    if ( ImGui::BeginChild("##channel-header", ImVec2{0.0F, height}, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar) ) {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("# %s",channel.m_name.c_str());
        ImGui::SameLine();
        if ( !server.m_name.empty() ) {
            ImGui::TextDisabled("on %s", server.m_name.c_str());
        }
    }
    ImGui::EndChild();
}

void UI::draw_message_history(const ChannelState& channel) {
    const bool stick_to_bottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0F;
    if ( channel.m_messages.empty() ) {
        ImGui::TextDisabled("No messages yet.");
        return;
    }
    for (std::size_t index = 0; index < channel.m_messages.size(); ++index) {
        const ChatMessage& message = channel.m_messages[index];
        ImGui::PushID(static_cast<int>(index));
        //sender
        ImGui::TextColored(ImVec4{0.35F, 0.65F, 1.0F, 1.0F}, "%s", message.m_sender.c_str());
        //Timestamp aligned to right
        //const std::string timestamp = format_time(message.m_timestamp);
        //const float timestamp_width = ImGui::CalcTextSize(timestamp.c_str()).x;
        //ImGui::SameLine();
        //ImGui::SetCursorPosX( std::max(ImGui::GetCursorPosX(), ImGui::GetWindowContentRegionMax().x - timestamp_width) );
        //ImGui::TextDisabled("%s", timestamp.c_str());
        //actual message
        ImGui::PushTextWrapPos(0.0F);
        ImGui::TextWrapped("%s", message.m_text.c_str());
        ImGui::PopTextWrapPos();
        ImGui::Spacing();
        ImGui::PopID();
    }
    if (stick_to_bottom) ImGui::SetScrollHereY(1.0F);
}

void UI::draw_composer(const ServerState& server, const ChannelState& channel, float height) {
    if ( !ImGui::BeginChild("##composer", ImVec2{0.0F, height}, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar) ) {
        ImGui::EndChild();
        return;
    }
    constexpr float send_button_width = 70.0F;
    const float input_width = std::max(1.0F, ImGui::GetContentRegionAvail().x - send_button_width - ImGui::GetStyle().ItemSpacing.x);
    ImGui::SetNextItemWidth(input_width);
    const std::string hint = "Message #" + channel.m_name;
    const bool pressed_enter = ImGui::InputTextWithHint("##message-input", hint.c_str(), &m_state.m_message_input, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    const bool pressed_send = ImGui::Button("Send", ImVec2{send_button_width, 0.0F});
    if ( (pressed_enter || pressed_send) && !m_state.m_message_input.empty() ) {
        std::string message = std::exchange(m_state.m_message_input, {});
        m_irc_client.sendMessage(server.m_server_id, channel.m_name, std::move(message));
        ImGui::SetKeyboardFocusHere(-1);
    }
    ImGui::EndChild();
}

void UI::draw_empty_chat() {
    const ImVec2 available = ImGui::GetContentRegionAvail();
    constexpr auto text = "Connect to a server and select a channel.";
    const ImVec2 text_size = ImGui::CalcTextSize(text);
    ImGui::SetCursorPos(
        ImVec2{
            std::max(0.0F,(available.x - text_size.x) * 0.5F),
            std::max(0.0F,(available.y - text_size.y) * 0.5F)
        }
    );
    ImGui::TextDisabled("%s", text);
}

const ServerState* UI::selected_server() const {
    const auto itr = m_irc_view_model.m_servers.find(m_state.m_selected_server_id);
    return itr == m_irc_view_model.m_servers.end() ? nullptr : &itr->second ;
}

const ChannelState* UI::selected_channel() const {
    const ServerState* server = selected_server();
    if ( !server || m_state.m_selected_channel.empty() ) { return nullptr; }
    const auto itr = server->m_channels.find(m_state.m_selected_channel);
    return itr == server->m_channels.end() ? nullptr : &itr->second ;
}

void UI::draw_pop_up_windows() {
    if ( m_state.m_show_join_server_window ) { this->draw_join_server_window(); }
}

void UI::draw_join_server_window() {
    const auto total_size = ImGui::GetMainViewport()->WorkSize;
    constexpr float window_size_x = 640;
    constexpr float window_size_y = 384;
    ImGui::SetNextWindowSize( ImVec2(window_size_x, window_size_y) );
    ImGui::SetNextWindowPos( ImVec2(total_size.x/2 - window_size_x/2,total_size.y/2 - window_size_y/2) );
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoCollapse |
                                       ImGuiWindowFlags_NoScrollbar;

    if ( !ImGui::Begin("Join Server", &m_state.m_show_join_server_window, flags) ){
        ImGui::End();
        return;
    }

    /*  Description */
    ImGui::TextUnformatted("Enter the IRC server connection information.");
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    /*  Server Hostname */
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputTextWithHint("##server", "irc.example.net", &m_state.m_join_server.m_host);
    ImGui::TextDisabled("Server");
    ImGui::Spacing();
    /*  Port */
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputTextWithHint("##port", "6667", &m_state.m_join_server.m_port);
    ImGui::TextDisabled("Port");
    ImGui::Spacing();
    /* Nickname */
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputTextWithHint("##nickname", "nickname", &m_state.m_join_server.m_nickname);
    ImGui::TextDisabled("Nickname");
    ImGui::Spacing();
    /* Optional  */
    if (ImGui::CollapsingHeader("Advanced")) {
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputTextWithHint("##username","defaults to nickname", &m_state.m_join_server.m_username);
        ImGui::TextDisabled("Username");
        ImGui::Spacing();
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputTextWithHint("##realname", "defaults to nickname", &m_state.m_join_server.m_real_name);
        ImGui::TextDisabled("Real name");
    }

    const float button_height = ImGui::GetFrameHeight();
    const float desired_y = ImGui::GetWindowHeight() - button_height - ImGui::GetStyle().WindowPadding.y;
    if (ImGui::GetCursorPosY() < desired_y) { ImGui::SetCursorPosY(desired_y); }
    constexpr float button_width = 120.0F;
    if (ImGui::Button("Cancel", ImVec2{button_width, 0.0F})) {

    }
    ImGui::SameLine();
    if ( ImGui::Button("Connect", ImVec2{button_width, 0.0F}) ) {
        m_irc_client.connect({
            .m_host = std::move(m_state.m_join_server.m_host),
            .m_port = std::move(m_state.m_join_server.m_port),
            .m_nick = std::move(m_state.m_join_server.m_nickname),
            .m_username = std::move(m_state.m_join_server.m_username)
        });
    }
    ImGui::End();
}
