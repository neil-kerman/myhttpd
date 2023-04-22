#ifndef HTTP_AUTHENTICATION_HPP
#define HTTP_AUTHENTICATION_HPP

#include <map>
#include <string>
#include <iostream>
#include <tinyxml2.h>

#include "user_role.hpp"
#include "user.hpp"
#include "../resource/request.hpp"
#include "../resource/response.hpp"

namespace myhttpd::session::http {

    class authentication {

    private:
        std::map<std::string, user_role> roles;

        std::map<std::string, user> users;

    private:
        std::string _to_based64_string(const std::string str);

    public:
        bool authenticate(request &req, response &rsp);

    public:
        authentication(tinyxml2::XMLElement* config);
    };
}

#endif // HTTP_AUTHENTICATION_HPP