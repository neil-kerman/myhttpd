#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <memory>
#include <unordered_map>
#include <boost/uuid/uuid_generators.hpp>

#include "session/session.hpp"
#include "session/server.hpp"
#include "resource.hpp"
#include "transceiver.hpp"
#include "network/connection.hpp"
#include "resource.hpp"

namespace myhttpd::session::http {

    class session: public myhttpd::session::session {

    private:
        resource& _resource;

    private:
        const boost::uuids::uuid _id;

        server& _server;

        std::unique_ptr<myhttpd::network::connection> _conn;

        transceiver _transceiver;

        boost::asio::deadline_timer _timer;

#ifdef PERFORMANCE_LOGGING

        std::map<std::string, std::chrono::steady_clock::time_point> _time_points;

        inline void _add_time_point(std::string name) {
            this->_time_points.insert(
                std::pair<std::string, std::chrono::steady_clock::time_point>(
                    name, std::chrono::high_resolution_clock::now()
                )
            );
        }

#endif

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

        void _wait_request_handler(const asio_error_code &error);

        void _wait_error_handler(const asio_error_code& error);

        void _receive_handler(const asio_error_code& error, std::shared_ptr<message> request);

        void _send_handler(const asio_error_code& error);

        void _request_resource_handler(std::shared_ptr<response> rsp);
    
    private:
        void _set_timer();

        void _wait_request();

        void _wait_error();

        void _receive();

        void _do_pre_process(std::shared_ptr<message> msg);

        void _request_resource(std::shared_ptr<request> req);

        void _do_post_process(std::shared_ptr<response> rsp);

        void _send(std::shared_ptr<response> rsp);

        void _terminate();

    public:
        virtual void start();

        virtual boost::uuids::uuid get_id();
        
    public:
        session(std::unique_ptr<myhttpd::network::connection> conn,
            resource& resource, boost::asio::io_context& ctx, myhttpd::session::server& ser);

#ifdef PERFORMANCE_LOGGING
        virtual ~session();
#else
        virtual ~session() = default;
#endif
    };
}


#endif // HTTP_SESSION_HPP
