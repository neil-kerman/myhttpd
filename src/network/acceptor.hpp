#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <memory>
#include <functional>

#include "connection.hpp"

namespace myhttpd::network {

    class acceptor {

    public:
        /* connection accept event handler */
        typedef std::function<void (const asio_error_code &error, std::unique_ptr<connection>)> accept_handler;

    public:
        virtual void async_accept(accept_handler handler) = 0;

    public:
        virtual ~acceptor() = default;
    };
}

#endif // ACCEPTOR_HPP
