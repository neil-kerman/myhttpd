#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <boost/asio/buffer.hpp>

namespace asio = boost::asio;

namespace myhttpd {
    class content {
    public:
        virtual ~content() {};
        virtual asio::const_buffer get_data();
    };
}

#endif // CONTENT_HPP
