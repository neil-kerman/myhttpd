#include <functional>
#include <memory>
#include <string>

#include "http_request.hpp"
#include "http_response.hpp"

namespace myhttpd {

    class rnode {

    public:
        enum error_code {
            success = 0,
            error = -1,
        };

        typedef std::function<void (error_code code, std::unique_ptr<http_response> rsp)> request_handler;

    public:
        virtual ~rnode() {}

        virtual void async_request(std::string url, std::unique_ptr<http_response> req) = 0;

    };
}