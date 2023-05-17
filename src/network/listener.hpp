#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <boost/asio.hpp>
#include <functional>
#include <memory>

#include "alias.hpp"
#include "connection.hpp"

namespace myhttpd::network {

    class listener {

    public:
        typedef std::function<void(std::unique_ptr<connection>)> acception_handler;

    public:
        virtual void start_async_accept(acception_handler handler) = 0;

    public:
        virtual ~listener() = default;
    };
}

#endif // LISTENER_HPP