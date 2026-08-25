#pragma once

#include "logger.h"
#include "window.h"
#include "container/threadSafeQueue.h"
#include "irc/ircClient.h"
#include "irc/ircMetadata.h"
#include "irc/ircNetworkService.h"
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
    Logger*             m_logger = &Logger::get();
    Window              m_window;
    IrcNetworkService   m_irc_network_service;
    ThreadSafeQueue<NetworkEventVariant> m_network_event_queue;
    SessionManager      m_session_manager;
    IrcClient           m_irc_client;
    UI                  m_ui;
    bool                m_is_running = true;
};
