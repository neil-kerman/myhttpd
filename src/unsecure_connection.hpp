#ifndef __UNSECURE_CONNECTION_H__
#define __UNSECURE_CONNECTION_H__

#include <boost/asio.hpp>

#include "connection.hpp"

class unsecured_connection: public connection {
private:
    boost::asio::ip::tcp::socket _soc;
};

#endif // __UNSECURE_CONNECTION_H__