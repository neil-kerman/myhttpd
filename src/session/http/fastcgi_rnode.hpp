#ifndef FASTCGI_RNODE_HPP
#define FASTCGI_RNODE_HPP

#include <boost/asio.hpp>
#include <list>
#include <map>

#include "rnode.hpp"
#include "fastcgi_definitions.hpp"

namespace myhttpd::http {

    class fastcgi_rnode : public rnode {

    private:
        std::list<unsigned short> _reqid_pool;

        

    public:
        virtual void async_request(std::shared_ptr<request> req, request_handler handler);

    public:
        fastcgi_rnode(std::string addr, int port);

        virtual ~fastcgi_rnode() = default;
    };
}

#endif // FILESYSTEM_RNODE_HPP
