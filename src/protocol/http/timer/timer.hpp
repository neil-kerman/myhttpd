#ifndef HTTP_TIMER_HPP
#define HTTP_TIMER_HPP

#include <string>

#include "timing_wheel.hpp"

namespace myhttpd::protocol::http {

    class timer {

    private:
        timing_wheel& _timing_wheel;

        timing_wheel::token _tk;

    public:
        std::string get_current_rfc1123_datetime();

        void set(timing_wheel::timeout_handler handler);

        void cancel();

    public:
        timer(timing_wheel& _timing_wheel);
    };
}

#endif // HTTP_TIMER_HPP