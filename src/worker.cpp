#include <boost/uuid/uuid_generators.hpp>

#include "worker.hpp"
#include "session/http/session.hpp"
#include "session/http/session_factory.hpp"

namespace myhttpd {

    void worker::_init_session_factories(tinyxml2::XMLElement* config) {

        auto http_config = config->FirstChildElement("http");
        std::unique_ptr<myhttpd::session::session_factory> fac =
            std::make_unique<session::http::session_factory>(http_config, this->_ctx, *this);
        std::pair < std::string, std::unique_ptr<session::session_factory>> pair("http", std::move(fac));
        this->_session_factories.insert(std::move(pair));
    }

    void worker::handle_connection(std::unique_ptr<network::connection> conn) {

        conn->reset_io_context(this->_ctx);
        auto& fac = this->_session_factories["http"];
        std::shared_ptr<session::session> ses = fac->create_session(std::move(conn));

        this->_ctx.post([ses, this]() {

            auto id = ses->get_id();
            this->_sessions.insert(std::pair<boost::uuids::uuid, std::shared_ptr<session::session>>(id, ses));
            ses->start();
        });
    }

    void worker::request_termination(session::session& sender) {

        auto id = sender.get_id();
        this->_sessions.erase(id);
    }

    void worker::start() {

        this->_working_thread.reset(new std::thread([this]() {
                    
            this->_ctx.run();
        }));
    }

    void worker::join() {

        this->_working_thread->join();
    }

    worker::worker(tinyxml2::XMLElement* config)
    : _work_guard(this->_ctx.get_executor()) {

        this->_init_session_factories(config);
    }
}