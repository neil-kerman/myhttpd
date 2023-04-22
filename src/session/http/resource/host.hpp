#ifndef HTTP_HOST_H
#define HTTP_HOST_H

#include <memory>
#include <functional>
#include <map>
#include <tinyxml2.h>
#include <list>

#include "rnode.hpp"
#include "../security/access_control.hpp"

namespace myhttpd::session::http {

    class host {

    public:
        typedef rnode::request_handler request_handler;

    private:
        access_control _ac;

        std::string _name;

        std::string _default;

        std::map<std::string, std::unique_ptr<rnode>> _rnodes;

        std::map<unsigned, std::shared_ptr<content>> _error_pages;

        std::unordered_map<std::string, std::string>& _mimedb;

    private:
        void _error_pages_init(tinyxml2::XMLElement* config);

        void _rnodes_init(tinyxml2::XMLElement* config);

    private:
        std::shared_ptr<response> _make_error(unsigned code);

    public:
        void async_request(std::shared_ptr<request> req, request_handler handler);

        void config(tinyxml2::XMLElement* config);

    public:
        host(
            std::map<unsigned, std::shared_ptr<content>> error_pages, 
            std::unordered_map<std::string, std::string> &mimedb
        );
    };
}

#endif // HTTP_HOST_H