#include <memory>

#include "manager.hpp"
#include "session.hpp"

namespace myhttpd::service::http {

    void manager::_init_id_pool() {

        const session_id pool_size = 2 << 16;
        this->_id_pool.reserve(pool_size);

        for (session_id i = 0; i < pool_size; i++) {

            this->_id_pool.push_back(i);
        }
    }

    resource& manager::get_resource() {
        
        return this->_resource;
    }

    void manager::create_session(
        std::unique_ptr<myhttpd::network::connection> conn) {

        auto id = this->_id_pool.back();
        this->_id_pool.pop_back();
        auto ses = std::make_unique<http::session>(
            std::move(conn),
            *this,
            timer(this->_timing_wheel)
        );
        auto& ses_ref = *ses;
        this->_sessions.insert(
            std::pair<session_id, std::unique_ptr<http::session>>(id, std::move(ses))
        );
        ses_ref.start([id, this]() {
            
            this->_id_pool.push_back(id);
            this->_sessions.erase(id);
        });
    }

    manager::manager(
        tinyxml2::XMLElement* config, 
        boost::asio::io_context& ctx, 
        worker& wk):
        _ctx(ctx),
        _timing_wheel(ctx),
        _worker(wk) {

        this->_init_id_pool();
        auto res_config = config->FirstChildElement("resource");
        this->_resource.config(res_config);
    }
}
