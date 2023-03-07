#ifndef FILESYSTEM_RNODE_HPP
#define FILESYSTEM_RNODE_HPP

#include <string>

#include "rnode.hpp"

namespace myhttpd {

    class filesystem_rnode: public rnode {

    private:
        std::string _path;

    public:
        filesystem_rnode(std::string path);

        virtual void async_request(std::string url, std::unique_ptr<http_request> req, request_handler handler);
    };
}

#endif // FILESYSTEM_RNODE_HPP
