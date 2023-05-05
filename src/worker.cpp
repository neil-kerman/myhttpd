#include <boost/uuid/uuid_generators.hpp>

#include "worker.hpp"
#include "protocol/http/session.hpp"
#include "protocol/http/manager.hpp"

namespace myhttpd {

    void worker::_init_session_factories(tinyxml2::XMLElement* config) {

        auto http_config = config->FirstChildElement("http");
        std::unique_ptr<myhttpd::protocol::manager> fac =
            std::make_unique<protocol::http::manager>(http_config, this->_ctx, *this);
        std::pair < std::string, std::unique_ptr<protocol::manager>> pair("http", std::move(fac));
        this->_session_factories.insert(std::move(pair));
    }

    void worker::handle_connection(std::unique_ptr<network::connection> conn) {

        conn->reset_io_context(this->_ctx);
        auto conn_sptr = std::make_shared<std::unique_ptr<network::connection>>(std::move(conn));

        this->_ctx.post([conn_sptr, this]() {

            auto& fac = this->_session_factories["http"];
            fac->create_session(std::move(*conn_sptr));
        });
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