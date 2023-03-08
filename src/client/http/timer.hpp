#include <functional>
#include <cstddef>
#include <boost/asio.hpp>

namespace myhttpd {

    class timer {
    
    public:
        enum status {
            canceled,
            timeout,
        };

        typedef std::function<void(status st)> timeout_handler;

    private:
        boost::asio::deadline_timer _t;

    public:
        timer(boost::asio::io_context& ctx);

        ~timer();

        void cancel();

        void async_timeout(unsigned int seconds, timeout_handler handler);
    };
}