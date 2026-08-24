#include "ircNetworkService.h"

void IRCNetworkService::init() {
    //start thread that execute jobs and waits for new ones
    m_io_thread = std::jthread([this]() {
        m_io_context.run();
    });
}

void IRCNetworkService::stop() {
    m_work_guard.reset();
}

IRCNetworkService::Executor IRCNetworkService::executor() {
    return m_io_context.get_executor();
}
