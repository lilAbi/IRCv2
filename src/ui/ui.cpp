#include "ui.h"

void UI::draw(ChatUiState &state,
    std::span<const Channel> channels,
    std::span<const ChatMessage> messages,
    std::span<const Member> members) {

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    constexpr ImGuiWindowFlags root_flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    // These styles are captured when the root window begins.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::PopStyleVar(3);

    if (ImGui::Begin("##irc-client-root",nullptr,root_flags)) {
        constexpr float sidebar_width = 220.0F;
        constexpr float header_height = 48.0F;
        constexpr float composer_height = 48.0F;
        //Left sidebar
        ImGui::PushStyleColor(ImGuiCol_ChildBg,k_sidebar_background);
        if ( ImGui::BeginChild("##sidebar",ImVec2{sidebar_width, 0.0F},ImGuiChildFlags_None) ) {
            this->draw_sidebar(state, channels);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        //Everything to the right of the sidebar
        ImGui::SameLine(0.0F, 0.0F);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0, 0});

        if (ImGui::BeginChild("##workspace", ImVec2{0.0F, 0.0F}, ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar)) {
            ImGui::PopStyleVar();
            std::string_view selected_channel = "no-channel";

            /*
            if ( state.selected_channel >= 0 && state.selected_channel < static_cast<int>(channels.size()) ){
                selected_channel = channels[state.selected_channel].name;
            }
            */

            this->draw_header(selected_channel, header_height);

             //Reserve enough room beneath the body for the composer.
            const float body_height = std::max(1.0F,ImGui::GetContentRegionAvail().y - composer_height - ImGui::GetStyle().ItemSpacing.y);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::PopStyleVar();

            if ( ImGui::BeginChild("##workspace-body", ImVec2{0.0F, body_height}, ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar) ) {
                constexpr ImGuiTableFlags body_flags =
                    ImGuiTableFlags_Resizable |
                    ImGuiTableFlags_BordersInnerV |
                    ImGuiTableFlags_SizingStretchProp;

                const ImVec2 body_size = ImGui::GetContentRegionAvail();

                if (ImGui::BeginTable("##body-columns",2, body_flags, body_size)) {
                    ImGui::TableSetupColumn("Chat", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Members", ImGuiTableColumnFlags_WidthFixed, 185.0F);
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, body_size.y);
                    ImGui::TableSetColumnIndex(0);
                    //draw_message_history(state, messages);
                    ImGui::TableSetColumnIndex(1);
                    //draw_member_list(state, members);
                    ImGui::EndTable();
                }
            }
            ImGui::EndChild();
            //draw_composer(state, composer_height, on_send);
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void UI::draw_header(std::string_view channel_name, float height) {
    if ( ImGui::BeginChild("##channel-header", ImVec2{0.0F, height}, ImGuiChildFlags_Borders, ImGuiWindowFlags_NoScrollbar) ) {
        if ( ImGui::BeginTable("##header-table",2, ImGuiTableFlags_SizingStretchProp) ) {
            ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 110.0F);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("# %.*s", static_cast<int>(channel_name.size()), channel_name.data());
            ImGui::SameLine();
            ImGui::TextDisabled("Welcome to the channel");
            ImGui::TableSetColumnIndex(1);
            ImGui::Button("...");
            ImGui::SameLine();
            ImGui::Button("Users");
            ImGui::EndTable();
        }
    }
    ImGui::EndChild();
}

void UI::draw_sidebar(ChatUiState& state, std::span<const Channel> channels) {
    ImGui::TextColored(ImVec4{1.0F, 0.65F, 0.15F, 1.0F},"MY IRC CLIENT");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4{0.35F, 0.85F, 0.50F, 1.0F},"Freenode");

    for (std::size_t index = 0; index < channels.size(); ++index) {
        ImGui::PushID(static_cast<int>(index));
        const bool selected = state.m_selected_channel == static_cast<int>(index);
        const std::string label = "# " + channels[index].m_name;
        ImGui::PushStyleColor(ImGuiCol_Header, k_selected_channel_background);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4{0.20F, 0.27F, 0.35F, 1.0F});
        ImGui::PushStyleColor(ImGuiCol_Text, selected ? ImVec4{1, 1, 1, 1} : k_sidebar_text);
        if ( ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_None, ImVec2{ImGui::GetContentRegionAvail().x, 0.0F}) ) {
            state.m_selected_channel = static_cast<int>(index);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    // Anchor controls near the bottom of the sidebar.
    const float controls_height = ImGui::GetFrameHeight();
    if ( ImGui::GetCursorPosY() + controls_height < ImGui::GetWindowHeight() - 20.0F ) {
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - controls_height - ImGui::GetStyle().WindowPadding.y);
    }
    ImGui::Button("+");
    ImGui::SameLine();
    ImGui::Button("Settings");
}