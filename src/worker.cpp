#include "worker.hpp"
#include "protocol/http/session.hpp"
#include "protocol/http/manager.hpp"

namespace myhttpd {

    using boost::asio::ip::tcp;

    void worker::_init_session_factories(tinyxml2::XMLElement* config) {

        auto http_config = config->FirstChildElement("http");
        std::unique_ptr<myhttpd::service::manager> fac =
            std::make_unique<service::http::manager>(http_config, this->_ctx, *this);
        std::pair < std::string, std::unique_ptr<service::manager>> pair("http", std::move(fac));
        this->_service_managers.insert(std::move(pair));
    }

    void worker::transfer_socket(std::unique_ptr<network::connection> conn) {
        
        auto new_conn = std::make_shared<std::unique_ptr<network::connection>>(std::move(conn));
        this->_ctx.post(

            [this, new_conn]() {

                (*new_conn)->async_init(this->_ctx, 
                
                    [this, new_conn](const asio_error_code error) {

                        this->_service_managers["http"]->create_session(std::move(*new_conn));
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
    }
}