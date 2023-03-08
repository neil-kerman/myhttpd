#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <cstddef>

namespace myhttpd::http {
    class content {

    public:
        virtual ~content() {}

        virtual const char* get_ptr() = 0;

        virtual std::size_t get_size() = 0;
    };
}

#endif // CONTENT_HPP
