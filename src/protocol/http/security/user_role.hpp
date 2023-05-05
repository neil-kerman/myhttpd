#ifndef HTTP_USER_ROLE_HPP
#define HTTP_USER_ROLE_HPP

#include <string>
#include <set>

namespace myhttpd::protocol::http {

    struct user_role {

        std::string name;
        std::set<std::string> include_by;
        std::set<std::string> includes;
    };
}

#endif // HTTP_ROLE_HPP