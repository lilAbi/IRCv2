#pragma once


/*
 *Owns a boost::asio::io_context, a work guard, and a background std::jthread running io_context.run().
 *It has methods executor() and post() to schedule work
 */

class IRCNetworkService {
};
