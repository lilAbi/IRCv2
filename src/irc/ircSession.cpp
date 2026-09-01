#include "ircSession.h"

IrcSession::IrcSession(const boost::asio::any_io_executor& io_executor, ThreadSafeQueue<NetworkEventVariant> &network_event_queue)
    : m_resolver(io_executor), m_socket(io_executor), m_network_event_queue(network_event_queue) {
}

void IrcSession::connect(ServerConfig config) {
    m_logger->trace("Attempting connection to endpoint {}", config.m_host);

    auto resolver_results = m_resolver.resolve(config.m_host, config.m_port);

    if ( resolver_results.max_size() != 0 ) {
        //resolver_results.begin()->endpoint();
    } else {
        m_logger->trace("resolver_resolts");
    }

    /*
    resolver_results->
    boost::asio::ip::tcp::endpoint endpoint{ boost::asio::ip::make_address_v4(config.m_host), static_cast<unsigned short>(stoi(config.m_port)) };
    m_socket.async_connect(*x.begin(), [](const boost::system::error_code& error){});
    m_logger->trace("Attempting connection to endpoint {}", config.m_host);
    */
    //endpoint.address().to_string()
    //dns lookup
    /*
    m_resolver.async_resolve(endpoint, [](const boost::system::error_code& error, const boost::asio::ip::tcp::resolver::results_type& results) {

    });
    */
    //m_resolver.resolve()
    //boost::asio::async_connect()
}

void IrcSession::disconnect() {

}

void IrcSession::join(std::string channel) {

}

void IrcSession::leave(std::string channel) {

}

void IrcSession::sendMessage(std::string target, std::string message) {

}

void IrcSession::onResolve() {

}

void IrcSession::onConnect() {

}

void IrcSession::onRead(std::size_t length, const boost::system::error_code &ec) {

}

void IrcSession::onWrite(std::size_t length, const boost::system::error_code &ec) {

}

void IrcSession::startRead() {

}

void IrcSession::startWrite() {

}
