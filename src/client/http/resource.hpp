#include <memory>
#include <functional>
#include <list>
#include <tinyxml2.h>

#include "rnode.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

namespace myhttpd {
    class resource {
    
    public:
        enum error_code {
            success = 0,
            error = -1,
        };

        typedef std::function<void (error_code code, std::unique_ptr<http_response> rsp)> request_handler;

    private:
        std::list<std::unique_ptr<rnode>> _rnodes;
            
    public:
        void async_request(std::unique_ptr<http_response> req, request_handler handler);

        void config(tinyxml2::XMLElement *config);
    };
}