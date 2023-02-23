#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <tinyxml2.h>

#include "connection.hpp"

class listener {
private:
    std::set<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> _handshakes;
    boost::asio::io_context &_io;
    boost::asio::ip::tcp::acceptor _ac;
    std::function<void (boost::asio::ip::tcp::socket peer)> _handler;
public:
    listener(boost::asio::io_context &io, tinyxml2::XMLElement *config);
    void listen();
    void async_accept(std::function<void (std::unique_ptr<connection>)> handler);
};
#endif // __LISTENER_H__