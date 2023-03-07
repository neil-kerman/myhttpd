#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <memory>
#include <functional>
#include <map>
#include <tinyxml2.h>

#include "rnode.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

namespace myhttpd {
    class resource {
    
    public:
        typedef std::function<void (std::unique_ptr<http_response> rsp)> request_handler;

    private:
        std::map<std::string ,std::unique_ptr<rnode>> _rnodes;
            
    public:
        void async_request(std::unique_ptr<http_request> req, request_handler handler);

        void config(tinyxml2::XMLElement *config);
    };
}

#endif // RESOURCE_HPP
