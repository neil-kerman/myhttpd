

#include "timer.hpp"

namespace myhttpd {
    timer::timer(boost::asio::io_context& ctx) 
    : _t(ctx){}

    timer::~timer() {

    }

    void timer::cancel() {
        this->_t.cancel();
    }

    void timer::async_timeout(unsigned int seconds, timeout_handler handler) {
        this->_t.expires_from_now(boost::posix_time::seconds(seconds));
        this->_t.async_wait(
            [handler](const boost::system::error_code& error) {
                if (error == boost::asio::error::operation_aborted) {
                    handler(canceled);
                } else {
                    handler(timeout);
                }
            }
        );
    }
}