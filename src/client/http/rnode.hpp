#include <functional>
#include <memory>
#include <string>

#include "http_request.hpp"
#include "http_response.hpp"

namespace myhttpd {

    class rnode {

    public:
        typedef std::function<void (std::unique_ptr<http_response> rsp)> request_handler;

    public:
        virtual ~rnode() {}

        virtual void async_request(std::string url, std::unique_ptr<http_request> req, request_handler handler) = 0;

    };
}