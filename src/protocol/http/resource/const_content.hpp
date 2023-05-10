#ifndef CONST_CONTENT_HPP
#define CONST_CONTENT_HPP

#include "../transceiver/content.hpp"

namespace myhttpd::service::http {

    class const_content: public content {

    private:
        const char* _data;

        std::size_t _size;

    public:
        static const_content from_c_string(const char* str);

    public:
        virtual std::size_t get_content_langth();

        virtual void async_read(read_handler handler, std::size_t offset = 0, std::size_t size = 0);

    public:
        const_content(const char* data, std::size_t size);

        virtual ~const_content() = default;
    };
}

#endif // CONST_CONTENT_HPP