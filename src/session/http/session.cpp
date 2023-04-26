#include <glog/logging.h>
#include <functional>
#ifdef PERFORMANCE_LOGGING
    #include <chrono>
#endif
#include <boost/date_time.hpp>
#include <boost/format.hpp>

#include "session.hpp"
#include "utils.hpp"

namespace myhttpd::session::http {

    void session::_wait_request_handler(const asio_error_code& error) {

        this->_transceiver_wait_busy = false;
        this->_timer.cancel();

        if (!error) {

            if (!this->_terminating_flag) {

                this->_receive();
            }

        } else {

            this->_terminate();
        }
    }

    void session::_wait_error_handler(const asio_error_code& error) {

        this->_terminate();
    }

    void session::_receive_handler(const asio_error_code& error, std::unique_ptr<message> msg) {

        this->_transceiver_receive_busy = false;

        if (!error) {

            if (!this->_terminating_flag) {

                this->_do_pre_process(std::move(msg));
            }

        } else {

            this->_terminate();
        }
    }

    void session::_request_resource_handler(std::unique_ptr<response> rsp) {

        this->_do_post_process(std::move(rsp));
    }

    void session::_send_handler(const asio_error_code& error) {

        this->_transceiver_send_busy = false;

        if (!error) {

            if (!this->_terminating_flag) {

                if (this->_keep_alive) {

                    this->_set_timer();
                    this->_wait_request();

                } else {

                    this->_terminate();
                }
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
        this->_timer.expires_from_now(boost::posix_time::seconds(10));
        this->_timer.async_wait(std::bind(&session::_timeout_handler, this, std::placeholders::_1));
    }

    void session::_wait_request() {

        this->_transceiver_wait_busy = true;
        this->_transceiver.async_wait(socket_wait_type::wait_read,
            std::bind(&session::_wait_request_handler, this, std::placeholders::_1));
    }

    void session::_wait_error() {

       this->_transceiver.async_wait(socket_wait_type::wait_error, 
           std::bind(&session::_wait_error_handler, this, std::placeholders::_1));
    }

    void session::_receive() {

        this->_transceiver_receive_busy = true;
        this->_transceiver.async_receive(
            std::bind(&session::_receive_handler, this, std::placeholders::_1, std::placeholders::_2));
    }

    void session::_do_pre_process(std::unique_ptr<message> msg) {

        auto req = std::make_unique<request>(std::move(*msg), this->_conn);

        if (req->contains_attribute("connection")) {

            if (message::assert_attribute(req->find_attribute("connection"), "keep-alive")) {

                this->_keep_alive = true;
            }
        }

        this->_request_resource(std::move(req));
    }

    void session::_request_resource(std::unique_ptr<request> req) {

        this->_resource.async_request(
            std::move(req), 
            std::bind(&session::_request_resource_handler, this, std::placeholders::_1)
        );
    }

    void session::_do_post_process(std::unique_ptr<response> rsp) {

        boost::posix_time::ptime utc_datetime = boost::posix_time::second_clock::universal_time();
        std::string datetime = utils::ptime_to_http_date(utc_datetime);
        rsp->insert_attribute("date", datetime);
        rsp->insert_attribute("server", "MyHttpd 0.1.0");

        if (this->_keep_alive) {

            rsp->insert_attribute("connection", "keep-alive");

        } else {

            rsp->insert_attribute("connection", "close");
        }

        this->_send(std::move(rsp));
    }

    void session::_send(std::unique_ptr<response> rsp) {

        this->_transceiver_send_busy = true;
        this->_transceiver.async_send(
            std::move(rsp), 
            std::bind(&session::_send_handler, this, std::placeholders::_1)
        );
    }

    void session::_terminate() {

        if (!(this->_transceiver_receive_busy || this->_transceiver_send_busy ||
            this->_transceiver_wait_busy || this->_timer_busy)) {

            this->_server.request_termination(*this);

        } else if (!this->_terminating_flag){

            if (this->_transceiver_receive_busy || this->_transceiver_send_busy ||
                this->_transceiver_wait_busy) {

                this->_transceiver.cancel();
            }

            if (this->_timer_busy) {

                this->_timer.cancel();
            }

            this->_terminating_flag = true;
        }
    }
    
    void session::start() {

        this->_set_timer();
        this->_wait_error();
        this->_wait_request();
    }

    boost::uuids::uuid session::get_id() {

        return this->_id;
    }

    session::session(
        std::unique_ptr<myhttpd::network::connection> conn,
        resource& resource,
        boost::asio::io_context& ctx,
        myhttpd::session::server& ser):
        _conn(std::move(conn)),
        _transceiver(this->_conn),
        _resource(resource),
        _timer(ctx),
        _server(ser),
        _id(boost::uuids::random_generator()()) {

    }
}
