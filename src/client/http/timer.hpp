#ifndef TIMER_HPP
#define TIMER_HPP

#include <functional>
#include <cstddef>
#include <boost/asio.hpp>

namespace myhttpd {

    class timer {
    
    public:
        typedef boost::system::error_code error_code;

        typedef std::function<void(const error_code & error_code)> timeout_handler;

    private:
        boost::asio::deadline_timer _t;

    public:
        timer(boost::asio::io_context& ctx);

        ~timer();

        void cancel();

        void async_timeout(unsigned int seconds, timeout_handler handler);
    };
}

#endif // TIMER_HPP
