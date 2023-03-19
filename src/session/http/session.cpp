#include <glog/logging.h>
#include <functional>
#include <chrono>

#include "session.hpp"
#include "const_content.hpp"

namespace myhttpd::http {

    void session::_wait_handler(const asio_error_code& error) {
        this->_transceiver_wait_busy = false;
        this->_timer.cancel();
        if (!error) {
            this->_receive();
        } else {
            this->_terminate();
        }
    }

    void session::_receive_handler(const asio_error_code& error, std::shared_ptr<message> msg) {
        this->_transceiver_receive_busy = false;
        if (!error) {
            this->_do_pre_process(msg);
        } else {
            this->_terminate();
        }
    }

    void session::_request_resource_handler(std::shared_ptr<response> rsp) {
        this->_do_post_process(rsp);
    }

    void session::_send_handler(const asio_error_code& error) {
        this->_transceiver_send_busy = false;
        if (!error) {
            if (this->_keep_alive) {
                this->_set_timer();
                this->_wait();
            } else {
                this->_terminate();
            }
        } else {
            this->_terminate();
        }
    }

    void session::_timeout_handler(const asio_error_code& error) {
        this->_timer_busy = false;
        if (error == asio_error::operation_aborted) {
        } else {
            this->_terminate();
        }
    }

    void session::_set_timer() {
        this->_timer_busy = true;
        this->_timer.expires_from_now(boost::posix_time::seconds(30));
        this->_timer.async_wait(std::bind(&session::_timeout_handler, this, std::placeholders::_1));
    }

    void session::_wait() {
        this->_transceiver_wait_busy = true;
        this->_transceiver.async_wait(socket_wait_type::wait_read,
            std::bind(&session::_wait_handler, this, std::placeholders::_1));
    }

    void session::_receive() {
        this->_transceiver_receive_busy = true;
        this->_transceiver.async_receive(
            std::bind(&session::_receive_handler, this, std::placeholders::_1, std::placeholders::_2));
    }

    void session::_do_pre_process(std::shared_ptr<message> msg) {
        auto req = std::make_shared<request>(std::move(*msg));
        if (req->contains_attribute("connection")) {
            if (message::assert_attribute(req->find_attribute("connection"), "keep-alive")) {
                this->_keep_alive = true;
            }
        }
        this->_request_resource(req);
    }

    void session::_request_resource(std::shared_ptr<request> req) {
        this->_resource.async_request(req, std::bind(&session::_request_resource_handler, this, std::placeholders::_1));
    }

    void session::_do_post_process(std::shared_ptr<response> rsp) {
        std::string datetime = std::format("{0:%a}, {0:%d} {0:%b} {0:%Y} {0:%H}:{0:%M}:{0:%S} GMT", std::chrono::utc_clock::now());
        rsp->insert_attribute("date", datetime);
        rsp->insert_attribute("server", "MyHttpd 0.1.0");
        if (this->_keep_alive) {
            rsp->insert_attribute("connection", "keep-alive");
        } else {
            rsp->insert_attribute("connection", "close");
        }
        this->_send(rsp);
    }

    void session::_send(std::shared_ptr<response> rsp) {
        this->_transceiver_send_busy = true;
        this->_transceiver.async_send(rsp, std::bind(&session::_send_handler, this, std::placeholders::_1));
    }

    void session::_terminate() {
        if (!this->_transceiver_receive_busy && !this->_transceiver_send_busy &&
            !this->_transceiver_wait_busy && !this->_timer_busy) {
            this->_terminated_handler();
        } else {
            if (this->_transceiver_receive_busy || this->_transceiver_send_busy ||
                this->_transceiver_wait_busy) {
                this->_transceiver.cancel();
            }
            if (this->_timer_busy) {
                this->_timer.cancel();
            }
            this->_terminating_required = true;
        }
    }
    
    void session::start(terminated_handler handler) {
        this->_terminated_handler = handler;
        this->_set_timer();
        this->_wait();
    }

    session::session(std::unique_ptr<myhttpd::network::connection> conn, resource &resource, boost::asio::io_context& ctx)
    : _conn(std::move(conn)), _transceiver(this->_conn), _resource(resource), _timer(ctx) {
        
    }
}
