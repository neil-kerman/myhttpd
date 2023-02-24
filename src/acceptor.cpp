#include <memory>
#include <functional>
#include <boost/bind.hpp>
#include <glog/logging.h>

#include "acceptor.hpp"
#include "connection.hpp"

void acceptor::_handshake_handler(std::unique_ptr<tls_soc> soc, const boost::system::error_code &error) {
    if(!error) {
        this->_nca_handler(std::make_unique<connection<tls_soc>>(std::move(*soc)));
    } else {
    }
}

void acceptor::_accept_handler_tls(const boost::system::error_code &error, ordinary_soc peer) {
    if(!error) {
        auto soc = new tls_soc(std::move(peer), *(this->_ssl_ctx));
        soc->async_handshake(
            tls_soc::server, boost::bind(
                &acceptor::_handshake_handler, 
                this, 
                std::unique_ptr<tls_soc>(soc),
                boost::placeholders::_1
            )
        );
        this->_ac->async_accept(boost::bind(&acceptor::_accept_handler_tls, this));
    } else {
    }
}

void acceptor::_accept_handler(const boost::system::error_code &error, ordinary_soc peer) {
    if(!error) {
        this->_nca_handler(std::make_unique<connection<ordinary_soc>>(std::move(peer)));
        this->_ac->async_accept(boost::bind(&acceptor::_accept_handler, this));
    } else {
    }
}

acceptor::acceptor(boost::asio::io_context &io, tinyxml2::XMLElement *config)
: _io(io) {
    std::string ip = config->Attribute("address");
    int port = config->IntAttribute("port");
    this->_ac.reset(new ip::tcp::acceptor(this->_io, ip::tcp::endpoint(ip::make_address(ip), port)));
    std::string tls = config->Attribute("tls");
    std::string certification_filepath = config->Attribute("certification");
}

void acceptor::start_async_accept(nca_handler handler) {
    this->_ac->listen();
    LOG(INFO) << "Listening at the local endpoint: " << this->_ac->local_endpoint();
    if(this->_is_tls) {
        this->_ac->async_accept(boost::bind(&acceptor::_accept_handler_tls, this));
    } else {
        this->_ac->async_accept(boost::bind(&acceptor::_accept_handler, this));
    }
}