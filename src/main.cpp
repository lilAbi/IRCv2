#include <memory>

#include "core/application.h"
#include "event/eventManager.h"

int main() {
    Logger* logger = &Logger::get();

    if (const std::unique_ptr<Application> application = std::make_unique<Application>(); application->init()) {
        logger->info("Application constructed at: {}", fmt::ptr(application.get()) );
        application->loop();
    } else {
        logger->critical("Application start-up failed");
    }

    return 0;
}
