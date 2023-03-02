#include <memory>
#include <functional>
#include <boost/bind/bind.hpp>
#include <glog/logging.h>

#include "acceptor.hpp"
#include "connection/insecure_connection.hpp"
#include "connection/secure_connection.hpp"

namespace myhttpd {

    namespace ip = boost::asio::ip;
    namespace ssl = boost::asio::ssl;

    void acceptor::_accept_handler_tls(const boost::system::error_code &error, boost::asio::ip::tcp::socket peer) {
        if(!error) {
            this->_ac->async_accept(boost::bind(&acceptor::_accept_handler_tls, this));
        } else {

        }
    }

    void acceptor::_accept_handler(const boost::system::error_code &error, boost::asio::ip::tcp::socket peer) {
        if(!error) {
            this->_nca_handler(std::make_unique<insecure_connection>(std::move(peer)));
            this->_ac->async_accept(boost::bind(&acceptor::_accept_handler, this));
        } else {

        }
    }

    acceptor::acceptor(boost::asio::io_context &io, tinyxml2::XMLElement *config)
    : _io(io) {
        std::string ip = config->Attribute("address");
        int port = config->IntAttribute("port");
        this->_ac.reset(new ip::tcp::acceptor(this->_io, ip::tcp::endpoint(ip::make_address(ip), port)));
        /* tls setting */
        this->_use_tls = false;
        if(config->Attribute("use_tls") && config->BoolAttribute("use_tls")) {
            this->_use_tls = true;
            std::string certification_filepath = config->Attribute("certification");
        }
    }

    void acceptor::start_async_accept(nca_handler handler) {
        this->_ac->listen();
        LOG(INFO) << "Listening at the local endpoint: " << this->_ac->local_endpoint();
        if(this->_use_tls) {
            this->_ac->async_accept(boost::bind(&acceptor::_accept_handler_tls, this));
        } else {
            this->_ac->async_accept(boost::bind(&acceptor::_accept_handler, this));
        }
    }
}