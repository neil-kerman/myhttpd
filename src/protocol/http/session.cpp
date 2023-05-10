#include "session.hpp"
#include "session.hpp"
#include <functional>
#include <boost/date_time.hpp>
#include <boost/format.hpp>

#include "session.hpp"
#include "utils.hpp"

namespace myhttpd::service::http {

    void session::_wait_handler(const transceiver::transceiver_error_code error) {

        this->_timer.cancel();

        if (!error) {

            this->_receive();

        } else {

            this->_terminate();
        }
    }

    void session::_receive_handler(const transceiver::transceiver_error_code error, std::unique_ptr<message> msg) {

        if (!error) {

            this->_do_pre_process(std::move(msg));

        } else {

            this->_terminate();
        }
    }

    void session::_send_handler(const transceiver::transceiver_error_code error) {

        if (!error) {

            this->_next_request();
        
        } else {

            this->_terminate();
        }
    }

    void session::_next_request() {

        if (this->_keep_alive) {

            this->_set_timer();
            this->_wait_request();

        } else {

            this->_terminate();
        }
    }

    void session::_set_timer() {

        this->_timer.set(
            
            [this]() {
            
                this->_terminate();
            }
        );
    }

    void session::_wait_request() {

        this->_transceiver.async_wait(
            socket_wait_type::wait_read, 
            std::bind(&http::session::_wait_handler, this, std::placeholders::_1)
        );
    }

    void session::_receive() {

        this->_transceiver.async_receive(
            std::bind(&http::session::_receive_handler, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    void session::_do_pre_process(std::unique_ptr<message> msg) {

        auto req = std::make_unique<request>(std::move(*msg), *(this->_conn));
        this->_keep_alive = req->keep_alive();
        this->_request_resource(std::move(req));
    }

    void session::_request_resource(std::unique_ptr<request> req) {

        this->_manager.get_resource().async_request(std::move(req), 
            
            [this](std::unique_ptr<response> rsp) {
                
                this->_do_post_process(std::move(rsp));
            }
        );
    }

    void session::_do_post_process(std::unique_ptr<response> rsp) {

        rsp->insert_attribute("date", this->_timer.get_current_rfc1123_datetime());
        rsp->insert_attribute("server", "MyHttpd 0.1.0");

        if (this->_keep_alive) {

            rsp->insert_attribute("connection", "keep-alive");

        } else {

            rsp->insert_attribute("connection", "close");
        }

        this->_send(std::move(rsp));
    }

    void session::_send(std::unique_ptr<response> rsp) {

        this->_transceiver.async_send(
            std::move(rsp), 
            std::bind(&http::session::_send_handler, this, std::placeholders::_1)
        );
    }

    void session::_terminate() {

        this->_transceiver.cancel(
            
            [this]() {
                
                this->_terminating_handler();
            }
        );
    }
    
    void session::start(terminating_handler handler) {

        this->_terminating_handler = handler;
        this->_set_timer();
        this->_wait_request();
    }

    session::session(std::unique_ptr<myhttpd::network::connection> conn, http::manager& manager, timer tmr)
    :_conn(std::move(conn)), _transceiver(*(this->_conn)), _manager(manager), _timer(tmr) {

    }
}
