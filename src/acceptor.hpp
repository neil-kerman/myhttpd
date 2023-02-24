#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <set>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <tinyxml2.h>

#include "connection.hpp"

namespace ip = boost::asio::ip;
namespace ssl = boost::asio::ssl;

typedef ssl::stream<ip::tcp::socket> tls_soc;
typedef ip::tcp::socket ordinary_soc;
/* New connection accepted event handler */
typedef std::function<void (std::unique_ptr<basic_connection>)> nca_handler;

class acceptor {
private:
    boost::asio::io_context &_io;
    std::unique_ptr<ip::tcp::acceptor> _ac;
    bool _is_tls;
    std::unique_ptr<ssl::context> _ssl_ctx;
    nca_handler _nca_handler;
private:
    void _handshake_handler(std::unique_ptr<tls_soc> soc, const boost::system::error_code& error);
    void _accept_handler_tls(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);
    void _accept_handler(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);
public:
    acceptor(boost::asio::io_context &io, tinyxml2::XMLElement *config);
    void start_async_accept(nca_handler handler);
};

#endif // ACCEPTOR_HPP
