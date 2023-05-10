#include <boost/uuid/uuid_generators.hpp>

#include "worker.hpp"
#include "network/handshaker_factory.hpp"
#include "protocol/http/session.hpp"
#include "protocol/http/manager.hpp"

namespace myhttpd {

    using boost::asio::ip::tcp;

    void worker::_init_session_factories(tinyxml2::XMLElement* config) {

        auto http_config = config->FirstChildElement("http");
        std::unique_ptr<myhttpd::service::manager> fac =
            std::make_unique<service::http::manager>(http_config, this->_ctx, *this);
        std::pair < std::string, std::unique_ptr<service::manager>> pair("http", std::move(fac));
        this->_session_factories.insert(std::move(pair));
    }

    void worker::_init_handshakers(tinyxml2::XMLElement* config) {

        auto acs_cfg = config->FirstChildElement("acceptors");
        auto ac_cfg = acs_cfg->FirstChildElement();

        /* Create handshakers */
        while (ac_cfg) {

            using namespace network;
            auto hs = handshaker_facory::create_handshakre_instance(ac_cfg);
            auto tag = hs->get_listener_tag();
            auto pair = std::pair<listener_tag, std::unique_ptr<handshaker>>(tag, std::move(hs));
            this->_handshakers.insert(std::move(pair));
            ac_cfg = ac_cfg->NextSiblingElement();
        }
    }

    tcp::socket worker::reset_io_context(tcp::socket soc) {

        auto protocol = soc.local_endpoint().protocol();
        auto soc_handle = soc.release();
        auto new_soc = tcp::socket(this->_ctx);
        new_soc.assign(protocol, soc_handle);
        return new_soc;
    }

    void worker::transfer_socket(network::listener_tag tag, tcp::socket soc) {
        
        auto new_soc_ptr = std::make_shared<tcp::socket>(this->reset_io_context(std::move(soc)));
        this->_ctx.post(

            [tag, new_soc_ptr, this]() {

                this->_handshakers[tag]->async_handshake(std::move(*new_soc_ptr),

                    [this](std::unique_ptr<network::connection> conn) {
                        
                        this->_session_factories["http"]->create_session(std::move(conn));
                    }
                );
            }
        );
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
        this->_init_handshakers(config);
    }
}