#ifndef HTTP_ACCESS_CONTROL_HPP
#define HTTP_ACCESS_CONTROL_HPP

#include <string>
#include <set>
#include <map>
#include <tinyxml2.h>

#include "user.hpp"
#include "authentication.hpp"
#include "../resource/request.hpp"
#include "../resource/response.hpp"
#include "../utils.hpp"

namespace myhttpd::service::http {

    template<typename Requestable, typename... Args> class access_control: Requestable {

    private:
        authentication& _auth;

        std::set<request::method> _owner_allowed_maths;

        std::set<request::method> _group_allowed_maths;

        std::set<request::method> _anonymous_allowed_maths;

        std::string _owner;

        std::string _group;

    public:
        virtual void async_request(std::unique_ptr<request> req, Requestable::request_handler handler) override {

            if (this->_anonymous_allowed_maths.count(req->get_method())) {

                Requestable::async_request(std::move(req), handler);
                return;
            }

            auto usr = this->_auth.authenticate(*req);

            if (usr == nullptr) {

                auto rsp = std::make_unique<response>(std::move(req));
                rsp->set_status(401);
                rsp->insert_attribute("www-authenticate", "Basic realm=\"Please input your name and password.\"");
                handler(std::move(rsp));
                return;

            } else {

                if (usr->groups.count(this->_group)) {

                    if (this->_group_allowed_maths.count(req->get_method())) {

                        Requestable::async_request(std::move(req), handler);
                        return;
                    }
                }

                if (usr->name == this->_owner) {

                    if (this->_owner_allowed_maths.count(req->get_method())) {

                        Requestable::async_request(std::move(req), handler);
                        return;
                    }
                }

                auto rsp = std::make_unique<response>(std::move(req));
                rsp->set_status(403);
                handler(std::move(rsp));
                return;
            }
        }

        access_control(tinyxml2::XMLElement* config, authentication& auth, Args... args)
            : Requestable(args...), _auth(auth) {

            auto ac = config->FirstChildElement("access_control");
            this->_owner = ac->Attribute("owner");
            this->_group = ac->Attribute("group");
            auto o_als = ac->Attribute("owner_allows");
            auto g_als = ac->Attribute("group_allows");
            auto a_als = ac->Attribute("anon_allows");
            auto o_als_l = utils::split(o_als, ",");
            auto g_als_l = utils::split(g_als, ",");
            auto a_als_l = utils::split(a_als, ",");

            for (auto& mt : a_als_l) {

                this->_anonymous_allowed_maths.insert(request::method_parse(mt));
            }

            this->_group_allowed_maths = this->_anonymous_allowed_maths;

            for (auto& mt : g_als_l) {

                this->_group_allowed_maths.insert(request::method_parse(mt));
            }

            this->_owner_allowed_maths = this->_group_allowed_maths;

            for (auto& mt : o_als_l) {

                this->_owner_allowed_maths.insert(request::method_parse(mt));
            }
        }
    };
}

#endif // ACCESS_CONTROL_HPP