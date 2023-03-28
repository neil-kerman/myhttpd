#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <functional>
#include <map>
#include <tinyxml2.h>
#include <list>

#include "rnode.hpp"

namespace myhttpd::http {

    class resource {
    
    public:
        typedef rnode::request_handler request_handler;

    private:
        std::map<std::string ,std::unique_ptr<rnode>> _rnodes;

        std::string _default;

        std::unordered_map<std::string, std::string> _mimedb = { {"default", "application/octet-stream"} };

        std::map<unsigned, std::shared_ptr<content>> _error_pages;

    private:
        void _error_pages_init();

        void _mimedb_init();

        void _rnodes_init(tinyxml2::XMLElement* config);

        std::shared_ptr<content> _get_error_page(unsigned status);

    public:
        void async_request(std::shared_ptr<request> req, request_handler handler);

        void config(tinyxml2::XMLElement *config);
    };
}

#endif // RESOURCE_HPP
