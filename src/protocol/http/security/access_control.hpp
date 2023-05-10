#ifndef HTTP_ACCESS_CONTROL_HPP
#define HTTP_ACCESS_CONTROL_HPP

#include <string>
#include <set>
#include <map>
#include <tinyxml2.h>

#include "user.hpp"
#include "user_role.hpp"
#include "../resource/request.hpp"
#include "../resource/response.hpp"

namespace myhttpd::service::http {

    class access_control {

    private:
        std::map<std::string, std::set<request::method>> _role_allowed_methods;

        std::set<request::method> _anonymous_allowed_methoeds;

    protected:
        void config(tinyxml2::XMLElement* config);

    public:
        bool check_permission(request& req, response& rsp);
    };
}

#endif // ACCESS_CONTROL_HPP