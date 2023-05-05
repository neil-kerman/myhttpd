#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <cstddef>
#include <functional>
#include <boost/system.hpp>

#include "network/connection.hpp"

namespace myhttpd::protocol::http {

    class content {

    public:
        enum content_error_code {

            success = 0,
            error = -1,
        };

        typedef std::function<void(const content_error_code error, network::connection::const_buffer buf)> read_handler;

    public:
        virtual std::size_t get_content_langth() = 0;

        virtual void async_read(read_handler handler, std::size_t offset = 0, std::size_t size = 0) = 0;

    public:
        virtual ~content() {}
    };
}

#endif // CONTENT_HPP
