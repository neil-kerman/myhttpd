#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <set>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <tinyxml2.h>

#include "connection/insecure_connection.hpp"
#include "connection/secure_connection.hpp"
namespace myhttpd {
    /* New connection accepted event handler */
    typedef std::function<void (std::unique_ptr<connection>)> nca_handler;
    class acceptor {
    private:
        boost::asio::io_context &_io;
        std::unique_ptr<boost::asio::ip::tcp::acceptor> _ac;
        std::unique_ptr<boost::asio::ssl::context> _tls_ctx;
        std::list<std::unique_ptr<secure_connection>> _handshaking_socs;
        nca_handler _nca_handler;
        bool _use_tls;
    private:
        void _accept_handler_tls(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);
        void _accept_handler(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);
    public:
        acceptor(boost::asio::io_context &io, tinyxml2::XMLElement *config);
        void start_async_accept(nca_handler handler);
    };
}

#endif // ACCEPTOR_HPP
