#ifndef HTTP_TIMING_WHEEL_HPP
#define HTTP_TIMING_WHEEL_HPP

#include <boost/asio.hpp>
#include <list>
#include <memory>
#include <string>

#include "../session_id.hpp"

namespace myhttpd::service::http {

    class timing_wheel {

    public:
        typedef std::function<void()> timeout_handler;

        struct timeout_handler_list {

            timeout_handler_list* previous;

            timeout_handler_list* next;

            timeout_handler handler;
        };

        typedef timeout_handler_list* token;

    private:
        boost::asio::io_context& _ctx;

        boost::asio::deadline_timer _raw_timer;

        std::list<timeout_handler_list> _timing_wheel;

        std::string _last_rfc1123_datetime;

    private:
        void _set_timer();

        void _update_datetime();

    public:
        void cancel(token tk);

        token set(timeout_handler handler);

        std::string get_current_rfc1123_datetime();

    public:
        timing_wheel(boost::asio::io_context& ctx);
    };
}

#endif // HTTP_TIMING_WHEEL_HPP