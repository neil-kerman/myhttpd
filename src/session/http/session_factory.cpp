#include <memory>

#include "session_factory.hpp"
#include "session.hpp"

namespace myhttpd::session::http {

    std::unique_ptr<myhttpd::session::session> session_factory::create_session(
        std::unique_ptr<myhttpd::network::connection> conn) {

        return std::make_unique<myhttpd::session::http::session>(
            std::move(conn), 
            this->_resource, 
            this->_ctx, 
            this->_server
        );
    }

    session_factory::session_factory(
        tinyxml2::XMLElement* config, 
        boost::asio::io_context& ctx, 
        server &ser): 
        _ctx(ctx), 
        _server(ser) {

        auto res_config = config->FirstChildElement("resource");
        this->_resource.config(res_config);
    }
}
