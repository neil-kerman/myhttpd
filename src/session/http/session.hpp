#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <memory>

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

        int _counter = 0;

    private:
        bool _transceiver_receive_busy = false;

        bool _transceiver_send_busy = false;

        bool _transceiver_wait_busy = false;

        bool _timer_busy = false;

        bool _terminating_required = false;

    private:
        void _set_timer();

        void _terminate();

    private:
        void _timeout_handler(const asio_error_code& error);

        void _wait_handler(const asio_error_code &error);

        void _receive_handler(const asio_error_code& error, std::unique_ptr<message> request);

        void _send_handler(const asio_error_code& error);

        void _resource_request_handler(std::shared_ptr<message> rsp);

    public:
        virtual void start(terminated_handler handler);
        
    public:
        session(std::unique_ptr<myhttpd::network::connection> conn, resource &resource, boost::asio::io_context &ctx);

        virtual ~session() = default;
    };
}


#endif // HTTP_SESSION_HPP
