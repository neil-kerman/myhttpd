#ifndef ALIAS_HPP
#define ALIAS_HPP

#include <boost/system.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/socket_base.hpp>

namespace asio_error = boost::asio::error;

typedef boost::system::error_code asio_error_code;

typedef boost::asio::socket_base::wait_type socket_wait_type;

#endif // ALIAS_HPP