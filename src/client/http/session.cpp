#include <glog/logging.h>
#include <functional>

#include "session.hpp"

namespace myhttpd::http {
    void session::_set_timer() {
        this->_timer_busy = true;
        this->_timer.expires_from_now(boost::posix_time::seconds(3));
        this->_timer.async_wait(std::bind(&session::_timeout_handler, this, std::placeholders::_1));
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

    void session::_timeout_handler(const asio_error_code& error) {
        this->_timer_busy = false;
        if (error == asio_error::operation_aborted) {
        } else {
            this->_terminate();
        }
    }

    void session::_wait_handler(const asio_error_code& error) {
        this->_transceiver_wait_busy = false;
        this->_timer.cancel();
        if (!error) {
            this->_transceiver_receive_busy = true;
            this->_transceiver.async_receive(
                std::bind(&session::_receive_handler, this, std::placeholders::_1, std::placeholders::_2));
        } else {
            this->_terminate();
        }
    }

    void session::_receive_handler(const asio_error_code& error, std::unique_ptr<message> request) {
        this->_transceiver_receive_busy = false;
        if (!error) {
            
        } else {
            this->_terminate();
        }
    }

    void session::_send_handler(const asio_error_code& error) {
        this->_transceiver_send_busy = false;
        if (!error) {
            this->_set_timer();
            this->_transceiver_wait_busy = true;
            this->_transceiver.async_wait(socket_wait_type::wait_read,
                std::bind(&session::_wait_handler, this, std::placeholders::_1));
        } else {
            this->_terminate();
        }
    }

    void session::start(terminated_handler handler) {
        this->_terminated_handler = handler;
        this->_set_timer();
        this->_transceiver_wait_busy = true;
        this->_transceiver.async_wait(socket_wait_type::wait_read,
            std::bind(&session::_wait_handler, this, std::placeholders::_1));
    }

    session::session(std::unique_ptr<myhttpd::network::connection> conn, resource &resource, boost::asio::io_context& ctx)
        : _transceiver(std::move(conn)), _resource(resource), _timer(ctx) {
    }

    session::~session() {}

}

