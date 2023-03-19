#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <memory>
#include <unordered_map>

#include "session/session.hpp"
#include "resource.hpp"
#include "transceiver.hpp"
#include "network/connection.hpp"
#include "resource.hpp"

namespace myhttpd::http {

    class session: public myhttpd::session {

    private:
        resource& _resource;

    private:
        std::unique_ptr<myhttpd::network::connection> _conn;

        transceiver _transceiver;

        boost::asio::deadline_timer _timer;

        terminated_handler _terminated_handler;

    private:
        bool _keep_alive = false;

    private:
        bool _transceiver_receive_busy = false;

        bool _transceiver_send_busy = false;

        bool _transceiver_wait_busy = false;

        bool _timer_busy = false;

        bool _terminating_required = false;

    private:
        void _timeout_handler(const asio_error_code& error);

        void _wait_handler(const asio_error_code &error);

        void _receive_handler(const asio_error_code& error, std::shared_ptr<message> request);

        void _send_handler(const asio_error_code& error);

        void _request_resource_handler(std::shared_ptr<response> rsp);
    
    private:
        void _set_timer();

        void _wait();

        void _receive();

        void _do_pre_process(std::shared_ptr<message> msg);

        void _request_resource(std::shared_ptr<request> req);

        void _do_post_process(std::shared_ptr<response> rsp);

        void _send(std::shared_ptr<response> rsp);

        void _terminate();

    public:
        virtual void start(terminated_handler handler);
        
    public:
        session(std::unique_ptr<myhttpd::network::connection> conn, resource &resource, boost::asio::io_context &ctx);

        virtual ~session() = default;
    };
}


#endif // HTTP_SESSION_HPP
