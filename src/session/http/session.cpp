#include <glog/logging.h>
#include <functional>
#include <chrono>

#include "session.hpp"
#include "const_content.hpp"

namespace myhttpd::http {

    void session::_wait_handler(const asio_error_code& error) {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("wait_t1");
#endif
        this->_transceiver_wait_busy = false;
        this->_timer.cancel();
        if (!error) {
            this->_receive();
        } else {
            this->_terminate();
        }
    }

    void session::_receive_handler(const asio_error_code& error, std::shared_ptr<message> msg) {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("receive_t1");
#endif
        this->_transceiver_receive_busy = false;
        if (!error) {
            this->_do_pre_process(msg);
        } else {
            this->_terminate();
        }
    }

    void session::_request_resource_handler(std::shared_ptr<response> rsp) {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("request_resource_t1");
#endif
        this->_do_post_process(rsp);
    }

    void session::_send_handler(const asio_error_code& error) {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("send_t1");
        auto t0 = this->_time_points["timeout_t0"];
        auto t1 = this->_time_points["timeout_t1"];
        std::chrono::duration<double, std::milli> dur = t1 - t0;
        DLOG(INFO) << "timeout time spendt: " << dur.count() << "ms";
        t0 = this->_time_points["wait_t0"];
        t1 = this->_time_points["wait_t1"];
        dur = t1 - t0;
        DLOG(INFO) << "wait time spendt: " << dur.count() << "ms";
        t0 = this->_time_points["receive_t0"];
        t1 = this->_time_points["receive_t1"];
        dur = t1 - t0;
        DLOG(INFO) << "receive time spendt: " << dur.count() << "ms";
        t0 = this->_time_points["request_resource_t0"];
        t1 = this->_time_points["request_resource_t1"];
        dur = t1 - t0;
        DLOG(INFO) << "request resource time spendt: " << dur.count() << "ms";
        t0 = this->_time_points["send_t0"];
        t1 = this->_time_points["send_t1"];
        dur = t1 - t0;
        DLOG(INFO) << "send time spendt: " << dur.count() << "ms";
#endif
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
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("timeout_t1");
#endif
        this->_timer_busy = false;
        if (error == asio_error::operation_aborted) {
        } else {
            this->_terminate();
        }
    }

    void session::_set_timer() {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("timeout_t0");
#endif
        this->_timer_busy = true;
        this->_timer.expires_from_now(boost::posix_time::seconds(120));
        this->_timer.async_wait(std::bind(&session::_timeout_handler, this, std::placeholders::_1));
    }

    void session::_wait() {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("wait_t0");
#endif
        this->_transceiver_wait_busy = true;
        this->_transceiver.async_wait(socket_wait_type::wait_read,
            std::bind(&session::_wait_handler, this, std::placeholders::_1));
    }

    void session::_receive() {
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("receive_t0");
#endif
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
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("request_resource_t0");
#endif
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
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("send_t0");
#endif
        this->_transceiver_send_busy = true;
        this->_transceiver.async_send(rsp, std::bind(&session::_send_handler, this, std::placeholders::_1));
    }

    void session::_terminate() {
        if (!this->_transceiver_receive_busy && !this->_transceiver_send_busy &&
            !this->_transceiver_wait_busy && !this->_timer_busy) {
#ifdef PERFORMANCE_LOGGING
            auto t0 = std::chrono::high_resolution_clock::now();
#endif
            this->_terminated_handler();
#ifdef PERFORMANCE_LOGGING
            auto t1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dur = t1 - t0;
            DLOG(INFO) << "session delete time spendt: " << dur.count() << "ms";
#endif
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
#ifdef PERFORMANCE_LOGGING
        this->_add_time_point("ses-start");
#endif
    }

#ifdef PERFORMANCE_LOGGING
    session::~session() {
        this->_add_time_point("ses-stop");
        auto t0 = this->_time_points["ses-start"];
        auto t1 = this->_time_points["ses-stop"];
        std::chrono::duration<double, std::milli> dur = t1 - t0;
        DLOG(INFO) << "Session time sepent: " << dur.count() << "ms";
    }
#endif
}
