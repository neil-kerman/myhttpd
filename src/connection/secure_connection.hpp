#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "connection.hpp"

namespace myhttpd::connection {
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> tls_soc;
    class secure_connection: public connection {
    private:
        tls_soc _soc;
    public:
        secure_connection(tls_soc soc);
        virtual ~secure_connection() {}
        virtual void close();
        virtual void async_write();
        virtual void async_read();
        virtual void async_wait();
        virtual std::size_t available();
    };
}
