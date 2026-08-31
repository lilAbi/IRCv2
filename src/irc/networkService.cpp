#include "networkService.h"

void NetworkService::init() {
    m_logger->info("Starting Network Service...");
    //run the io_context in a background thread
    m_io_thread = std::jthread([this]() {
        m_logger->get_network_logger()->info("Background jthread worker started...");
        try {
            //run the io_context object's event processing loop.
            m_io_context.run();
        } catch (const std::exception& payload) {
            m_logger->get_network_logger()->critical("caught: {}", payload.what());
        }
        m_logger->get_network_logger()->info("Finished processing io_context object's event processing loop");
    });
    m_logger->trace("Network Service Initialized");
}

void NetworkService::stop() {
    m_logger->info("Shutting down Network Service...");
    m_work_guard.reset();
    m_io_thread.join();
    m_logger->info("Network Service background thread joined");
}

NetworkService::Executor NetworkService::get_executor() {
    return m_io_context.get_executor();
}
