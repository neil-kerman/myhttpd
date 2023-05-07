

#include "timer.hpp"

namespace myhttpd::protocol::http {

    std::string timer::get_current_rfc1123_datetime() {

        return this->_timing_wheel.get_current_rfc1123_datetime();
    }

    void timer::set(timing_wheel::timeout_handler handler) {

        this->_tk = this->_timing_wheel.set(handler);
    }

    void timer::cancel() {

        this->_timing_wheel.cancel(this->_tk);
    }

    timer::timer(timing_wheel& wheel) : _timing_wheel(wheel) {

    }
}