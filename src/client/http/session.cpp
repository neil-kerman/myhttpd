#include <glog/logging.h>
#include <functional>

#include "session.hpp"

namespace myhttpd::http {

    void session::_terminate() {
        if (!this->_transceiver_receive_busy && !this->_transceiver_send_busy &&
        !this->_transceiver_wait_busy && !this->_timer_busy) {
            this->_terminated_handler();
        } else {
            if (this->_transceiver_receive_busy || this->_transceiver_send_busy ||
            this->_transceiver_wait_busy) {
                this->_transceiver->cancel();
            }
            if (this->_timer_busy) {
                this->_timer.cancel();
            }
            this->_terminating_required = true;
        }
    }

    void session::_timeout_handler(timer::status st) {
        this->_timer_busy = false;
        if (st == timer::canceled) {
        } else {
            this->_terminate();
        }
    }

    void session::_wait_handler(transceiver::error_code code) {
        this->_transceiver_wait_busy = false;
        this->_timer.cancel();
        if (!code) {
            this->_transceiver_receive_busy = true;
            this->_transceiver->async_receive(
                std::bind(&session::_receive_handler, this, std::placeholders::_1, std::placeholders::_2));
        } else {
            this->_terminate();
        }
    }

    void session::_receive_handler(transceiver::error_code code, std::unique_ptr<message> request) {
        this->_transceiver_receive_busy = false;
        if (!code) {
            LOG(INFO) << request->get_method();
            LOG(INFO) << request->get_url();
            LOG(INFO) << request->get_version();
            LOG(INFO) << request->get_first_attribute("HOST");
            LOG(INFO) << request->get_first_attribute("connection");
            LOG(INFO) << request->get_first_attribute("user-agent");
            auto rsp = std::make_unique<message>();
            rsp->header.push_back("HTTP/1.1 200 Ok\r\n");
            rsp->header.push_back("Content-Length: 13\r\n");
            rsp->header.push_back("\r\n");
            rsp->header.push_back("Hello client.");
            this->_transceiver_send_busy = true;
            this->_transceiver->async_send(std::move(rsp), std::bind(&session::_send_handler, this, std::placeholders::_1));
        } else {
            this->_terminate();
        }
    }

    void session::_send_handler(transceiver::error_code code) {
        this->_transceiver_send_busy = false;
        if (!code) {
            this->_timer_busy = true;
            this->_timer.async_timeout(3, std::bind(&session::_timeout_handler, this, std::placeholders::_1));
            this->_transceiver_wait_busy = true;
            this->_transceiver->async_wait(transceiver::wait_type::wait_receive,
                std::bind(&session::_wait_handler, this, std::placeholders::_1));
        } else {
            this->_terminate();
        }
    }

    void session::start(terminated_handler handler) {
        this->_terminated_handler = handler;
        this->_timer_busy = true;
        this->_timer.async_timeout(3, std::bind(&session::_timeout_handler, this, std::placeholders::_1));
        this->_transceiver_wait_busy = true;
        this->_transceiver->async_wait(transceiver::wait_type::wait_receive,
            std::bind(&session::_wait_handler, this, std::placeholders::_1));
    }

    session::session(std::unique_ptr<connection> conn, resource &resource, boost::asio::io_context& ctx)
        : _conn(std::move(conn)), _transceiver(std::make_unique<transceiver_1_1>(this->_conn)), _resource(resource),_timer(ctx) {
    }

    session::~session() {}

}
