#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "server.hpp"
#include "network/acceptor_factory.hpp"
#include "session/http/session.hpp"
#include "session/http/session_factory.hpp"

using namespace boost::asio::ip;

namespace myhttpd {

    void server::_init_acceptors(tinyxml2::XMLElement* config) {

        auto acs_cfg = config->FirstChildElement("acceptors");
        auto ac_cfg = acs_cfg->FirstChildElement();

        while (ac_cfg) {

            /* Create acceptors */
            this->_acceptors.push_back(network::acceptor_facory::create_acceptor(ac_cfg, this->_ctx, *this));
            ac_cfg = ac_cfg->NextSiblingElement();
        }
    }

    void server::pass_connection(std::unique_ptr<network::connection> conn) {

        this->_workers.front()->handle_connection(std::move(conn));
        this->_workers.push_back(std::move(this->_workers.front()));
        this->_workers.pop_front();
    }

    void server::start() {

        for (auto& worker : this->_workers) {

            worker->start();
        }

        for (auto& ac : this->_acceptors) {

            ac->start_async_accept();
        }

        this->_thread.reset(new std::thread([this]() {
            
            this->_ctx.run();
        }));
    }

    void server::join() {

        this->_thread->join();
    }

    server::server(tinyxml2::XMLElement *config)
    : _work_guard(this->_ctx.get_executor()) {

        this->_init_acceptors(config);
        auto worker_num = std::thread::hardware_concurrency() * 2;

        for (int i = 0; i < worker_num; i++) {

            this->_workers.push_back(std::make_unique<worker>(config));
        }
    }
}

