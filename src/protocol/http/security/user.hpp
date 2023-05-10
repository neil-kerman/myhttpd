#ifndef HTTP_USER_HPP
#define HTTP_USER_HPP

#include <string>
#include <set>

namespace myhttpd::service::http {

    struct user {
        
        std::string name;

        std::string password;

        std::set<std::string> groups;
    };
}

#endif // HTTP_USER_HPP