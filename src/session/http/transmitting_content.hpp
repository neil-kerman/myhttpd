#ifndef TRANSMITTING_CONTENT_HPP
#define TRANSMITTING_CONTENT_HPP

#include <memory>
#include <list>
#include <vector>

#include "content.hpp"

namespace myhttpd::http {

    class transmitting_content : public content {

    private:
        bool _is_ready = false;

        asio_error_code _error;

        std::shared_ptr<std::vector<char>> _data;

        std::list<wait_handler> _wait_list;

    public:
        void deliver(const asio_error_code& error);

    public:
        virtual std::size_t get_size();

        virtual void async_wait_ready(wait_handler handler);

    public:
        transmitting_content() = delete;

        transmitting_content(std::shared_ptr<std::vector<char>> data);

        virtual ~transmitting_content() = default;
    };
}

#endif // TRANSMITTING_CONTENT_HPP