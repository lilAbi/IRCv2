#pragma once

#include <thread>
#include <boost/asio.hpp>

#include "core/logger.h"

/*
 *Owns a boost::asio::io_context, a work guard, and a background std::jthread running io_context.run().
 *It has methods executor() and post() to schedule work
 */

class IrcNetworkService {
    using Executor = boost::asio::io_context::executor_type;
public:
    //initialize the "network service"
    void init();
    //return from "run()"
    void stop();
    //get executor
    Executor executor();
    //submit "job"
    template<typename Func>
    void post(Func&& function);
private:
    Logger*                 m_logger = &Logger::get();
    std::jthread            m_io_thread;
    boost::asio::io_context m_io_context;
    boost::asio::executor_work_guard<Executor> m_work_guard{boost::asio::make_work_guard(m_io_context)};
};

template<typename Func>
void IrcNetworkService::post(Func&& function) {
    boost::asio::post( m_io_context, std::function<Func>(function) );
}
