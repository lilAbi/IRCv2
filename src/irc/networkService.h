#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include "core/logger.h"

/*
 *Owns a boost::asio::io_context, a work guard, and a background std::jthread running io_context.run().
 *It has methods executor() and post() to schedule work
 */

//not really a "NetworkService"

class NetworkService {
    using Executor = boost::asio::io_context::executor_type;
public:
    //initialize the "network service"
    void init();
    //return from "run()"
    void stop();
    //get executor
    Executor get_executor();
    //submit "job"
    template<typename Func>
    void post(Func&& func);
private:
    Logger*                 m_logger = &Logger::get();
    std::jthread            m_io_thread;
    boost::asio::io_context m_io_context;
    boost::asio::executor_work_guard<Executor> m_work_guard{boost::asio::make_work_guard(m_io_context)};
};

template<typename Func>
void NetworkService::post(Func&& func) {
    boost::asio::post( m_io_context, std::forward<Func>(func) );
}
