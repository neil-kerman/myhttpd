#include <boost/asio/buffer.hpp>

namespace asio = boost::asio;

namespace myhttpd::http {
    class content {
    public:
        virtual ~content() {};
        virtual asio::const_buffer get_data();
    };
}