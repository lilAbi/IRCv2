#pragma once

#include "logger.h"
#include "window.h"
#include "container/threadSafeQueue.h"
#include "irc/applicationController.h"
#include "irc/ircClient.h"
#include "irc/ircMetadata.h"
#include "irc/ircNetworkService.h"
#include "irc/ircViewModel.h"
#include "irc/sessionManager.h"
#include "ui/ui.h"

/*
 *  Manages application state and coordinates sub-systems
 */

class Application {
public:
    Application() = default;
    ~Application();

    //initialize the subsystems
    bool init();

    //application loop
    void loop();

    //delete copy/move assignment/constructors
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

private:
    //event loop
    void handle_sdl_event(const SDL_Event& event);

private:
    Logger*                 m_logger = &Logger::get();
    Window                  m_window;
    IrcNetworkService       m_irc_network_service;
    IrcViewModel            m_irc_view_model;
    ThreadSafeQueue<NetworkEventVariant> m_network_event_queue;
    ApplicationController   m_application_controller{m_irc_view_model, m_network_event_queue};
    SessionManager          m_session_manager{m_irc_network_service.get_executor(), m_network_event_queue};
    IrcClient               m_irc_client{m_irc_network_service, m_session_manager};
    UI                      m_ui{m_irc_client};
    bool                    m_is_running = true;
};
