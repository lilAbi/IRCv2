#include "ircNetworkService.h"

void IrcNetworkService::init() {
    m_logger->info("Starting IRC Network Service...");
    //start thread that execute jobs and waits for new ones
    m_io_thread = std::jthread([this]() {
        m_logger->get_network_logger()->info("Background jthread worker started...");
        m_io_context.run();
        m_logger->get_network_logger()->info("Returned from \"run()\"");
    });
}

void IrcNetworkService::stop() {
    m_logger->info("Shutting down IRC Network Service...");
    m_work_guard.reset();
}

IrcNetworkService::Executor IrcNetworkService::executor() {
    return m_io_context.get_executor();
}
