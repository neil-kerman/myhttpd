#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <boost/asio.hpp>

#include "connection.hpp"

class listener {
private:
    boost::asio::io_context &_io;
    boost::asio::ip::tcp::acceptor _ac;
    std::function<void (boost::asio::ip::tcp::socket peer)> _handler;
public:
    listener(boost::asio::io_context &io, boost::asio::ip::tcp::endpoint endp);
    listener(listener &&lis);
private:
};
#endif // __LISTENER_H__