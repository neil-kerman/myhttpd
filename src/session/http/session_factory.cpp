#include <memory>

#include "session_factory.hpp"
#include "session.hpp"

namespace myhttpd::session::http {

    std::unique_ptr<myhttpd::session::session> session_factory::create_session(std::unique_ptr<myhttpd::network::connection> conn) {
        return std::make_unique<myhttpd::session::http::session>(std::move(conn), this->_resource, this->_ctx);
    }

    session_factory::session_factory(tinyxml2::XMLElement* config, boost::asio::io_context& ctx)
    : _ctx(ctx){
        auto res_config = config->FirstChildElement("resource");
        this->_resource.config(res_config);
    }
}
