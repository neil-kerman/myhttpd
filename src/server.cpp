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

    void server::event_loop() {
        this->_io.run();
    }

    void server::_init_acceptors(tinyxml2::XMLElement* config) {
        auto acs_cfg = config->FirstChildElement("acceptors");
        auto ac_cfg = acs_cfg->FirstChildElement();
        //The memory of XMLElement objects are managed by the XMLDocument object which created themslevs,
        //Thus no need to delete it in this scope.
        while (ac_cfg) {
            /* Create acceptors */
            this->_acceptors.push_back(network::acceptor_facory::create_acceptor(ac_cfg, this->_io));
            ac_cfg = ac_cfg->NextSiblingElement();
        }
    }

    void server::_init_session_factories(tinyxml2::XMLElement* config) {
        auto http_config = config->FirstChildElement("http");
        std::unique_ptr<myhttpd::session_factory> fac = 
            std::make_unique<http::session_factory>(http_config, this->_io);
        std::pair < std::string, std::unique_ptr<myhttpd::session_factory>> pair("http", std::move(fac));
        this->_session_factories.insert(std::move(pair));
    }

    server::server(tinyxml2::XMLElement *config)
    : _work_guard(this->_io.get_executor()) {
        this->_init_acceptors(config);
        this->_init_session_factories(config);
    }

    void server::start() {
        this->_accept_handlers.clear();
        this->_accept_handlers.reserve(this->_acceptors.size());
        auto ac_it = this->_acceptors.begin();
        for (int i = 0; i < this->_acceptors.size(); i++) {
            auto handler = [&handlers = this->_accept_handlers, handler_id = i, &ac = (*ac_it), 
                &sessions = this->_sessions, &factories = this->_session_factories]
            (const asio_error_code& error, std::unique_ptr<network::connection> conn) {
                boost::uuids::uuid id = boost::uuids::random_generator()();
                auto& fac = factories["http"];
                auto ses = fac->create_session(std::move(conn));
                sessions.insert(std::pair<boost::uuids::uuid, std::unique_ptr<session>>(id, std::move(ses)));
                sessions[id]->start(
                    [id, &sessions]() {
                        sessions.erase(id);
                    }
                );
                ac->async_accept(handlers[handler_id]);
            };
            this->_accept_handlers.push_back(handler);
            (*ac_it)->async_accept(handler);
            ac_it++;
        }
    }
}

