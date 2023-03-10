#ifndef FILESYSTEM_RNODE_HPP
#define FILESYSTEM_RNODE_HPP

#include <string>
#include <boost/asio.hpp>

#include "rnode.hpp"
#include "message.hpp"

namespace myhttpd::http {

    class filesystem_rnode: public rnode {

    private:
        std::string _path;

    public:
        virtual bool exists(std::string url);

        virtual void async_request(std::string url, std::unique_ptr<message> req, request_handler handler);

    public:
        filesystem_rnode(std::string path);

        virtual ~filesystem_rnode();
    };
}

#endif // FILESYSTEM_RNODE_HPP
