#include <boost/uuid/uuid_generators.hpp>
#include <glog/logging.h>

#include "acceptor.hpp"
#include "insecure_connection.hpp"
#include "secure_connection.hpp"

namespace myhttpd {

    void acceptor::_accept_handler_tls(const boost::system::error_code &error, boost::asio::ip::tcp::socket peer) {
        if(!error) {
            boost::uuids::uuid id = boost::uuids::random_generator()();
            this->_tls_conns.insert(
                std::pair<boost::uuids::uuid, std::unique_ptr<secure_connection>>(
                    id, std::make_unique<secure_connection>(this->_tls_ctx, std::move(peer))
                )
            );
            this->_tls_conns[id]->async_handshake(
                [id, &conns = this->_tls_conns, handler = this->_nca_handler](connection::error_code error) {
                    if(!error) {
                        handler(std::move(conns[id]));
                    } else {

                    }
                }
            );
            using namespace std::placeholders;
            this->_ac.async_accept(std::bind(&acceptor::_accept_handler_tls, this, _1, _2));
        } else {
            
        }
    }

    void acceptor::_accept_handler(const boost::system::error_code &error, boost::asio::ip::tcp::socket peer) {
        if(!error) {
            this->_nca_handler(std::make_unique<insecure_connection>(std::move(peer)));
            using namespace std::placeholders;
            this->_ac.async_accept(std::bind(&acceptor::_accept_handler, this, _1, _2));
        } else {

        }
    }

    boost::asio::ssl::context acceptor::_init_tls(tinyxml2::XMLElement *config) {
        using namespace boost::asio::ssl;
        this->_use_tls = false;
        if(config->Attribute("use_tls") && config->BoolAttribute("use_tls")) {
            this->_use_tls = true;
            auto ctx = context(context::tlsv12_server);
            std::string crt_filepath = config->Attribute("certificate_file");
            std::string key_filepath = config->Attribute("key_file");
            ctx.use_certificate_file(crt_filepath, context::file_format::pem);
            ctx.use_private_key_file(key_filepath, context::file_format::pem);
            return std::move(ctx);
        } else {
            auto ctx = context(context::tlsv12_server);
            return std::move(ctx);
        }
    }

    boost::asio::ip::tcp::acceptor acceptor::_init_acceptor(tinyxml2::XMLElement *config) {
        namespace ip = boost::asio::ip;
        std::string addr = config->Attribute("address");
        int port = config->IntAttribute("port");
        return ip::tcp::acceptor(this->_io, ip::tcp::endpoint(ip::make_address(addr), port));
    }

    acceptor::acceptor(boost::asio::io_context &io, tinyxml2::XMLElement *config)
    : _io(io), _ac(this->_init_acceptor(config)), _tls_ctx(this->_init_tls(config)){
        
    }

    void acceptor::start_async_accept(nca_handler handler) {
        this->_nca_handler = handler;
        this->_ac.listen();
        LOG(INFO) << "Listening at the local endpoint: " << this->_ac.local_endpoint();
        using namespace std::placeholders;
        if(this->_use_tls) {
            this->_ac.async_accept(std::bind(&acceptor::_accept_handler_tls,this, _1, _2));
        } else {
            this->_ac.async_accept(std::bind(&acceptor::_accept_handler, this, _1, _2));
        }
    }
}