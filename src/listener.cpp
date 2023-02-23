#include <boost/bind.hpp>
#include <glog/logging.h>
#include <functional>

#include "listener.hpp"
#include "connection.hpp"

using namespace boost::asio::ip;

tcp::endpoint get_endpoint(tinyxml2::XMLElement *config) {
    std::string ip = config->Attribute("address");
    int port = config->IntAttribute("port");
    return tcp::endpoint(make_address(ip), port);
}

listener::listener(boost::asio::io_context &io, tinyxml2::XMLElement *config)
: _io(io), _ac(io, get_endpoint(config)) {
    std::string tls = config->Attribute("tls");
    std::string certification_filepath = config->Attribute("certification");
}

void listener::listen() {
    this->_ac.listen();
    LOG(INFO) << "Listening at the local endpoint: " << this->_ac.local_endpoint();
}

void listener::async_accept(std::function<void (std::unique_ptr<connection>)> handler) {
    this->_ac.async_accept([handler](const boost::system::error_code& error, tcp::socket peer) {
        if (!error) {
            std::unique_ptr<connection> conn = std::make_unique<unsecure_connection>(std::move(peer));
            handler(std::move(conn));
        }
    });
}