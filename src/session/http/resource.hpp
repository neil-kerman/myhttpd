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

    public:
        void async_request(std::shared_ptr<request> req, request_handler handler);

        void config(tinyxml2::XMLElement *config);
    };
}

#endif // RESOURCE_HPP
