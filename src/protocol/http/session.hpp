#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <memory>
#include <unordered_map>
#include <boost/uuid/uuid_generators.hpp>

#include "../session.hpp"
#include "network/connection.hpp"

#include "manager.hpp"
#include "transceiver/transceiver.hpp"
#include "timer/timer.hpp"

namespace myhttpd::protocol::http {

    class session: public myhttpd::protocol::session {

    private:
        http::manager& _manager;

    private:
        std::unique_ptr<myhttpd::network::connection> _conn;

        transceiver _transceiver;

        terminating_handler _terminating_handler;

    private:
        bool _keep_alive = false;

    private:
        void _wait_handler(const transceiver::transceiver_error_code error);

        void _receive_handler(const transceiver::transceiver_error_code error, std::unique_ptr<message> request);

        void _send_handler(const transceiver::transceiver_error_code error);

    private:
        void _next_request();

        void _wait_request();

        void _receive();

        void _do_pre_process(std::unique_ptr<message> msg);

        void _request_resource(std::unique_ptr<request> req);

        void _do_post_process(std::unique_ptr<response> rsp);

        void _send(std::unique_ptr<response> rsp);

        void _terminate();

    public:
        virtual void start(terminating_handler handler);

    public:
        session(std::unique_ptr<myhttpd::network::connection> conn, http::manager &manager);

        virtual ~session() = default;
    };
}


#endif // HTTP_SESSION_HPP
