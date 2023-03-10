#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <cstddef>
#include <functional>

namespace myhttpd::http {
    class content {

    public:
        enum error_code {
            success = 0,
            canceled,
            error,
        };

        typedef std::function<void(error_code code, char *data, std::size_t size)> wait_handler;

    public:
        virtual void async_wait_ready(wait_handler) = 0;

    public:
        virtual ~content() {}
    };
}

#endif // CONTENT_HPP
