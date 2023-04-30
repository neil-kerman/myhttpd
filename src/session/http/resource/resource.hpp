#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <functional>
#include <unordered_map>
#include <tinyxml2.h>
#include <list>

#include "host.hpp"

namespace myhttpd::session::http {

    class resource {
    
    public:
        typedef rnode::request_handler request_handler;

    private:
        std::unordered_map<std::string , host> _hosts;

        std::unordered_map<std::string, std::string> _mimedb = { {"default", "application/octet-stream"} };

        std::array<std::shared_ptr<content>, 506> _error_pages;

    private:
        void _hosts_init(tinyxml2::XMLElement* config);

        void _error_pages_init(tinyxml2::XMLElement* config);

        void _mimedb_init();

    private:
        std::unique_ptr<response> _make_error(unsigned code, std::unique_ptr<request> req);

    public:
        void async_request(std::unique_ptr<request> req, request_handler handler);

        void config(tinyxml2::XMLElement* config);
    };
}

#endif // RESOURCE_HPP
