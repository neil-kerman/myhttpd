#ifndef HTTP_MANAGER_HPP
#define HTTP_MANAGER_HPP

#include <tinyxml2.h>
#include <boost/asio/io_context.hpp>
#include <vector>
#include <map>

#include "../manager.hpp"
#include "../../worker.hpp"
#include "resource/resource.hpp"

namespace myhttpd::protocol::http{

    class manager : public protocol::manager {

    public:
        struct session_config {

            unsigned int keepalive_time = 30;
        };

        typedef int session_id;

    private:
        boost::asio::io_context& _ctx;

        resource _resource;

        std::vector<session_id> _id_pool;

        std::map<session_id, std::shared_ptr<protocol::session>> _sessions;

        session_config _session_config;

        worker& _worker;

    private:
        void _init_id_pool();

    public:
        resource& get_resource();

        virtual void create_session(std::unique_ptr<myhttpd::network::connection> conn);

    public:
        manager(tinyxml2::XMLElement* config, boost::asio::io_context& ctx, worker &wk);

        virtual ~manager() = default;
    };
}

#endif // HTTP_MANAGER_HPP
