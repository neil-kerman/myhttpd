#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <cstddef>
#include <functional>
#include <boost/system.hpp>

#include "network/connection.hpp"

namespace myhttpd::session::http {

    class content {

    public:
        enum error_code {
            successful = 0,
            error,
        };

        typedef std::function<void(const asio_error_code &error, network::connection::const_buffer buf)> wait_handler;

    public:
        virtual std::size_t get_size() = 0;

        virtual const void* get_data() = 0;

        virtual void async_wait_ready(wait_handler handler) = 0;

    public:
        virtual ~content() {}
    };
}

#endif // CONTENT_HPP
