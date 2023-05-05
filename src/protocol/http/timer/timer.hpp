#ifndef HTTP_TIMER_HPP
#define HTTP_TIMER_HPP

#include <string>

#include "timer_service.hpp"

namespace myhttpd::protocol::http {

    class timer {

    private:
        timer_service& _service;

    public:
        std::string get_current_rfc1123_datetime();

        void set_timer();

        void cancel();

    public:
        timer(timer_service& service);
    };
}

#endif // HTTP_TIMER_HPP