#include <memory>

#include "session_factory.hpp"
#include "session.hpp"

namespace myhttpd::http {

    std::unique_ptr<myhttpd::session> session_factory::create_session(std::unique_ptr<myhttpd::connection> conn) {
        std::unique_ptr<myhttpd::session> ses = std::make_unique<myhttpd::http::session>(std::move(conn), this->_ctx);
        return ses;
    }

    session_factory::session_factory(tinyxml2::XMLElement* config, boost::asio::io_context& ctx)
    : _ctx(ctx){

    }

    session_factory::~session_factory() {}
}
