#include <string>

#include "rnode.hpp"

namespace myhttpd {

    class filesystem_rnode: public rnode {

    public:
        filesystem_rnode(std::string path);

        virtual void async_request(std::string url, std::unique_ptr<http_response> req);
    };
}