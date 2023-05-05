#ifndef HTTP_USER_HPP
#define HTTP_USER_HPP

#include <string>

#include "user_role.hpp"

namespace myhttpd::protocol::http {

    struct user {
        
        bool anonymous;
        std::string name;
        std::string password;
        user_role& role;
    };
}

#endif // HTTP_USER_HPP