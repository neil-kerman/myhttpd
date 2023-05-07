#ifndef HTTP_MANAGER_HPP
#define HTTP_MANAGER_HPP

#include <tinyxml2.h>
#include <boost/asio/io_context.hpp>
#include <vector>
#include <map>

#include "../manager.hpp"
#include "../../worker.hpp"
#include "resource/resource.hpp"
#include "timer/timing_wheel.hpp"

namespace myhttpd::protocol::http{

    class manager : public protocol::manager {

    private:
        boost::asio::io_context& _ctx;

        timing_wheel _timing_wheel;

        resource _resource;

        std::vector<session_id> _id_pool;

        std::map<session_id, std::shared_ptr<protocol::session>> _sessions;

        worker& _worker;

    private:
        void _init_id_pool();

    public:
        resource& get_resource();

    
    public:
        virtual void create_session(std::unique_ptr<myhttpd::network::connection> conn);

    public:
        manager(tinyxml2::XMLElement* config, boost::asio::io_context& ctx, worker &wk);

        virtual ~manager() = default;
    };
}

#endif // HTTP_MANAGER_HPP
