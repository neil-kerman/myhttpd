#ifndef FILESYSTEM_RNODE_HPP
#define FILESYSTEM_RNODE_HPP

#include <string>
#include <boost/asio.hpp>

#include "../rnode.hpp"

namespace myhttpd::session::http {

    class filesystem_rnode: public rnode {

    private:
        std::string _path;

    private:
        bool _exists(std::string url);

        void _do_option(std::shared_ptr<request> req, request_handler handler);

        void _do_get(std::shared_ptr<request> req, request_handler handler);

        void _do_head(std::shared_ptr<request> req, request_handler handler);

        void _do_post(std::shared_ptr<request> req, request_handler handler);

        void _do_put(std::shared_ptr<request> req, request_handler handler);

        void _do_delete(std::shared_ptr<request> req, request_handler handler);

        void _do_trace(std::shared_ptr<request> req, request_handler handler);

    public:
        virtual void async_request(std::shared_ptr<request> req, request_handler handler);

    public:
        filesystem_rnode(std::string path);

        virtual ~filesystem_rnode() = default;
    };
}

#endif // FILESYSTEM_RNODE_HPP
