#ifndef FILESYSTEM_RNODE_HPP
#define FILESYSTEM_RNODE_HPP

#include <string>
#include <boost/asio.hpp>

#include "rnode.hpp"
#include "request.hpp"

namespace myhttpd::http {

    class filesystem_rnode: public rnode {

    private:
        std::string _path;

    public:
        filesystem_rnode(std::string path);

        void async_request(std::string url, std::unique_ptr<request> req, request_handler handler);
    };
}

#endif // FILESYSTEM_RNODE_HPP
