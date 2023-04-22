#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <functional>
#include <map>
#include <tinyxml2.h>
#include <list>

#include "host.hpp"

namespace myhttpd::session::http {

    class resource {
    
    public:
        typedef rnode::request_handler request_handler;

    private:
        std::map<std::string , host> _hosts;

        std::unordered_map<std::string, std::string> _mimedb = { {"default", "application/octet-stream"} };

        std::map<unsigned, std::shared_ptr<content>> _error_pages;

    private:
        void _hosts_init(tinyxml2::XMLElement* config);

        void _error_pages_init(tinyxml2::XMLElement* config);

        void _mimedb_init();

    private:
        std::shared_ptr<response> _make_error(unsigned code);

    public:
        void async_request(std::shared_ptr<request> req, request_handler handler);

        void config(tinyxml2::XMLElement* config);
    };
}

#endif // RESOURCE_HPP
