#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <cstddef>
#include <functional>
#include <boost/system.hpp>

namespace myhttpd::http {

    class content {

    public:
        enum error_code {
            successful = 0,
            error,
        };

        typedef std::function<void(const error_code error, void *data, std::size_t size)> wait_handler;

    public:
        virtual void async_wait_ready(wait_handler handler) = 0;

    public:
        virtual ~content() {}
    };
}

#endif // CONTENT_HPP
