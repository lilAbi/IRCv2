#include "ui.h"

#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

void UI::draw() {
    //draw main window
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    //set the next window size to the total viewport area
    const ImGuiViewport* viewport = ImGui::GetMainViewport(); //get the full viewport
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    //draw initial root window
    if (ImGui::Begin("Root", nullptr, flags)) {

        //draw menu bar
        this->draw_menu_bar();
        ImGui::Separator();

        //draw button header
        this->draw_button_header();
        ImGui::Separator();

        ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        ImGui::Spacing();

        ImGui::End(); //end of root
    }
}

void UI::draw_menu_bar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Tasks")){
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("File")){
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Downloads")){
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")){
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")){
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void UI::draw_button_header() {
    // create a popup window to "add a download" to the queue
    if (ImGui::Button("Add URL")) {
        m_shared_ui_window_data.m_show_add_download_window = !m_shared_ui_window_data.m_show_add_download_window;
    }; ImGui::SameLine();

    ImGui::Button("Resume"); ImGui::SameLine();
    ImGui::Button("Stop"); ImGui::SameLine();
    ImGui::Button("Delete"); ImGui::SameLine();
    ImGui::Button("Delete Completed"); ImGui::SameLine();
    ImGui::Button("Scheduler"); ImGui::SameLine();
    ImGui::Button("Options"); ImGui::SameLine();
    ImGui::Button("Start Queue"); ImGui::SameLine();
    ImGui::Button("Stop Queue"); ImGui::SameLine();
    ImGui::Button("Grabber");
}