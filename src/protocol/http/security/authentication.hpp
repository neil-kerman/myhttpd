#ifndef HTTP_AUTHENTICATION_HPP
#define HTTP_AUTHENTICATION_HPP

#include <map>
#include <string>
#include <iostream>
#include <tinyxml2.h>

#include "user.hpp"
#include "../resource/request.hpp"
#include "../resource/response.hpp"

namespace myhttpd::service::http {

    class authentication {

    private:
        std::map<std::string, std::shared_ptr<user>> _users;

    private:
        static void base64_decode(const std::string& input, std::string& output);

    public:
        std::shared_ptr<user> authenticate(request &req);

    public:
        authentication(tinyxml2::XMLElement* config);
    };
}

#endif // HTTP_AUTHENTICATION_HPP