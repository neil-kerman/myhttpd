
#include "server.hpp"
#include "network/listener_factory.hpp"

namespace myhttpd {

    void server::_init_listeners(tinyxml2::XMLElement* config) {

        auto listeners_config = config->FirstChildElement("acceptors");
        auto lis_cfg = listeners_config->FirstChildElement();

        /* Create liastener */
        while (lis_cfg) {
            
            auto lis = network::listener_factory::create_listenr(lis_cfg, this->_ctx);
            this->_listeners.push_back(std::move(lis));
            lis_cfg = lis_cfg->NextSiblingElement();
        }
    }

    void server::_init_workers(tinyxml2::XMLElement* config) {

#ifdef DEBUG

        auto worker_num = 1;

#else

        auto worker_num = std::thread::hardware_concurrency();
        //auto worker_num = 1;

#endif

        /* Create workers */
        for (int i = 0; i < worker_num; i++) {

            this->_workers.push_back(std::make_unique<worker>(config));
        }
    }

    void server::start() {

        for (auto& worker : this->_workers) {

            worker->start();
        }

        for (auto& lis : this->_listeners) {

            lis->start_async_accept(
                
                [this](std::unique_ptr<network::connection> conn) {
                
                    this->_workers.front()->transfer_socket(std::move(conn));
                    this->_workers.push_back(std::move(this->_workers.front()));
                    this->_workers.pop_front();
                }
            );
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

        this->_init_listeners(config);
        this->_init_workers(config);
    }
}

