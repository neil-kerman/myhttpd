#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <memory>
#include <functional>

#include "connection.hpp"

namespace myhttpd::network {

    class acceptor {

    public:
        virtual void start_async_accept() = 0;

    public:
        virtual ~acceptor() = default;
    };
}

#endif // ACCEPTOR_HPP
