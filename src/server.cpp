
#include "server.hpp"
#include "network/handshaker_factory.hpp"

using namespace boost::asio::ip;

namespace myhttpd {

    void server::_init_acceptors(tinyxml2::XMLElement* config) {

        auto acs_cfg = config->FirstChildElement("acceptors");
        auto ac_cfg = acs_cfg->FirstChildElement();

        /* Create acceptors */
        while (ac_cfg) {
            
            this->_listeners.push_back(network::listener::create_listener(ac_cfg, this->_ctx));
            ac_cfg = ac_cfg->NextSiblingElement();
        }
    }

    void server::_init_workers(tinyxml2::XMLElement* config) {

#ifdef DEBUG

        auto worker_num = 1;

#else

        auto worker_num = std::thread::hardware_concurrency();

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

        for (auto& listener : this->_listeners) {

            listener.start_async_accept(
                
                [this, tag = listener.get_listener_tag()](boost::asio::ip::tcp::socket soc) {
                
                    this->_workers.front()->transfer_socket(tag, std::move(soc));
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

        this->_init_acceptors(config);
        this->_init_workers(config);
    }
}

