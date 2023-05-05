#ifndef TRANSMITTING_CONTENT_HPP
#define TRANSMITTING_CONTENT_HPP

#include <memory>
#include <list>
#include <vector>

#include "content.hpp"

namespace myhttpd::protocol::http {

    class transmitting_content : public content {

    private:
        std::vector<char> _buffer;

        std::size_t _available_bytes = 0;

        bool _requested = false;

        std::size_t _requested_size;

        std::size_t _requested_offset;

        read_handler _read_handler;

    public:
        network::connection::mutable_buffer get_buffer(std::size_t size = 0);

        void cancel();
        
        std::size_t commit(std::size_t size);

    public:
        virtual std::size_t get_content_langth();

        virtual void async_read(read_handler handler, std::size_t offset = 0, std::size_t size = 0);

    public:
        transmitting_content() = delete;

        transmitting_content(std::size_t size);

        virtual ~transmitting_content() = default;
    };
}

#endif // TRANSMITTING_CONTENT_HPP