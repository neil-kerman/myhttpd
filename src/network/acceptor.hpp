#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <map>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/uuid/uuid.hpp>
#include <tinyxml2.h>

#include "insecure_connection.hpp"
#include "secure_connection.hpp"

namespace myhttpd {
    
    class acceptor {

    public:
        /* New connection accepted event handler */
        typedef std::function<void (std::unique_ptr<connection>)> nca_handler;

    private:
        boost::asio::io_context &_io;

        boost::asio::ip::tcp::acceptor _ac;

        boost::asio::ssl::context _tls_ctx;

        std::map<boost::uuids::uuid, std::unique_ptr<secure_connection>> _tls_conns;

        nca_handler _nca_handler;

        bool _use_tls;

    private:
        boost::asio::ssl::context _init_tls(tinyxml2::XMLElement *config);

        boost::asio::ip::tcp::acceptor _init_acceptor(tinyxml2::XMLElement *config);

    private:
        void _accept_handler_tls(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);

        void _accept_handler(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);

    public:
        acceptor(boost::asio::io_context &io, tinyxml2::XMLElement *config);

        void start_async_accept(nca_handler handler);
        
    };
}

#endif // ACCEPTOR_HPP
