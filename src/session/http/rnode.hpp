#ifndef RNODE_HPP
#define RNODE_HPP

#include <functional>
#include <memory>
#include <string>

#include "request.hpp"
#include "response.hpp"

namespace myhttpd::session::http {

    class rnode {

    public:
        typedef std::function<void (std::shared_ptr<response> rsp)> request_handler;

    public:
        virtual void async_request(std::shared_ptr<request> req, request_handler handler) = 0;

    public:
        virtual ~rnode() = default;
    };
}

#endif // RNODE_HPP
