#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/uuid/uuid_generators.hpp>

#include "server.hpp"
#include "network/acceptor_factory.hpp"
#include "session/http/session.hpp"
#include "session/http/session_factory.hpp"

using namespace boost::asio::ip;

namespace myhttpd {

    void server::_init_acceptors(tinyxml2::XMLElement* config) {
        auto acs_cfg = config->FirstChildElement("acceptors");
        auto ac_cfg = acs_cfg->FirstChildElement();
        //The memory of XMLElement objects are managed by the XMLDocument object which created themselves,
        //Thus no need to delete it in this scope.
        while (ac_cfg) {
            /* Create acceptors */
            this->_acceptors.push_back(network::acceptor_facory::create_acceptor(ac_cfg, this->_ctx, *this));
            ac_cfg = ac_cfg->NextSiblingElement();
        }
    }

    void server::_init_session_factories(tinyxml2::XMLElement* config) {
        auto http_config = config->FirstChildElement("http");
        std::unique_ptr<myhttpd::session::session_factory> fac =
            std::make_unique<session::http::session_factory>(http_config, this->_ctx, (*this));
        std::pair < std::string, std::unique_ptr<session::session_factory>> pair("http", std::move(fac));
        this->_session_factories.insert(std::move(pair));
    }

    void server::pass_connection(std::unique_ptr<network::connection> conn) {
        auto& fac = this->_session_factories["http"];
        auto ses = fac->create_session(std::move(conn));
        ses->start();
        auto id = ses->get_id();
        this->_sessions.insert(std::pair<boost::uuids::uuid, std::shared_ptr<session::session>>(id, std::move(ses)));
    }

    void server::request_termination(session::session& sender) {
        this->_sessions.erase(sender.get_id());
    }

    void server::event_loop() {
        this->_ctx.run();
    }

    server::server(tinyxml2::XMLElement *config)
    : _work_guard(this->_ctx.get_executor()) {
        this->_init_acceptors(config);
        this->_init_session_factories(config);
    }

    void server::start() {
        for (auto &ac: this->_acceptors) {
            ac->start_async_accept();
        }
    }
}

