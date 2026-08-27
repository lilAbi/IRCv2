#include "ircNetworkService.h"

void IrcNetworkService::init() {
    m_logger->info("Starting IRC Network Service...");
    //create a new thread- dedicated to processing aiso event queue
    m_io_thread = std::jthread([this]() {
        m_logger->get_network_logger()->info("Background jthread worker started...");
        try {
            //enters the event loop and blocks the current thread until the queued up work completes
            m_io_context.run();
        } catch (const std::exception& payload) {
            m_logger->get_network_logger()->critical("caught: {}", payload.what());
        }
        m_logger->get_network_logger()->info("Returned from \"run()\"");
    });
}

void IrcNetworkService::stop() {
    m_logger->info("Shutting down IRC Network Service...");
    m_work_guard.reset();
    m_io_thread.join();
    m_logger->info("Background thread joined");
}

IrcNetworkService::Executor IrcNetworkService::get_executor() {
    return m_io_context.get_executor();
}
