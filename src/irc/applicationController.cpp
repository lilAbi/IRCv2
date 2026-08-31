#include "applicationController.h"

ApplicationController::ApplicationController(IrcViewModel& view_model, ThreadSafeQueue<NetworkEventVariant>& network_event_queue)
    : m_view_model(view_model), m_network_event_queue(network_event_queue) {
}

void ApplicationController::process_events() {
    m_network_event_queue.drain([this] (NetworkEventVariant network_event_variant) -> void {
        try {
            std::visit(
                [this](auto&& event) {
                    this->handleEvent(event);
                },
                network_event_variant
            );
        } catch (const std::exception& exception) {
            m_logger->critical( "Unhandled exception while processing ApplicationController::process_events(): {}", exception.what() );
        } catch (...) {
            m_logger->critical("Unknown exception while processing ApplicationController::process_events()");
        }
    });
}

void ApplicationController::handleEvent(ConnectedEvent event) {
    m_logger->info("ConnectedEvent Called");
}

void ApplicationController::handleEvent(DisconnectedEvent event) {
    m_logger->info("DisconnectedEvent Called");
}

void ApplicationController::handleEvent(ChannelJoinedEvent event) {
    m_logger->info("ChannelJoinedEvent Called");
}

void ApplicationController::handleEvent(ChannelLeftEvent event) {
    m_logger->info("ChannelLeftEvent Called");
}

void ApplicationController::handleEvent(MessageReceivedEvent event) {
    m_logger->info("MessageReceivedEvent Called");
}
