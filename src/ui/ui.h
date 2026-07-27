#pragma once
#include "core/logger.h"
#include "event/eventManager.h"

//Data to be shared across different functions of the ui manager
struct UIWindowData {
    //enable popup windows
    bool            m_show_add_download_window = false;
    //Other data
    //download source address
    std::string     m_source = "https://cdn.truefilesize.com/test/test-100mb.zip";
    std::string     m_output = "test-100mb.zip";
};

//Should own the view model
class UI {
public:
    void draw();
private:
    void draw_menu_bar();

    void draw_button_header();

private:
    inline static UIWindowData  m_shared_ui_window_data;
    Logger*                     m_logger = &Logger::get();
    EventManager*               m_event_manager = &EventManager::get();
};
