#ifndef CONST_CONTENT_HPP
#define CONST_CONTENT_HPP

#include "content.hpp"

namespace myhttpd::session::http {

    class const_content: public content {

    private:
        const void* _data;

        std::size_t _size;

    public:
        static const_content from_c_string(const char* str);

    public:
        virtual std::size_t get_size();

        virtual const void* get_data();

        virtual void async_wait_ready(wait_handler handler);

    public:
        const_content(const void* data, std::size_t size);

        virtual ~const_content() = default;
    };
}

#endif // CONST_CONTENT_HPP