#ifndef HTTP_ACCESS_CONTROL_HPP
#define HTTP_ACCESS_CONTROL_HPP

#include <string>
#include <set>
#include <algorithm>
#include <tinyxml2.h>

#include "user.hpp"
#include "authentication.hpp"
#include "../resource/request.hpp"
#include "../resource/response.hpp"
#include "../utils.hpp"

namespace myhttpd::service::http {

    template<typename Requestable, typename... Args> class access_control: Requestable {

    private:
        enum identity_type { 

            owner,
            group,
            anonymous,
        };

    private:
        authentication& _auth;

        std::set<request::method> _owner_allowable_meths;

        std::set<request::method> _group_allowable_meths;

        std::set<request::method> _anonymous_allowable_meths;

        bool _has_access_controlling;

        bool _has_owner;

        bool _has_group;

        std::string _owner;

        std::string _group;

    private:
        void _invoke_base_impl(identity_type type, std::unique_ptr<request> req, Requestable::request_handler handler) {

            if (req->get_method() == request::method::OPTIONS) {

                Requestable::async_request(std::move(req),

                    [this, type, handler](std::unique_ptr<response> rsp) {

                        if (!rsp->contains_attribute("allows")) {

                            handler(std::move(rsp));
                            return;
                        }

                        auto al_attr = rsp->find_attribute("allows");
                        auto al_attr_val = al_attr->second;
                        rsp->erase_attribute(al_attr);
                        auto splited = utils::split(al_attr_val, ",");
                        std::set<request::method> base_allowed_meths;

                        for (auto& meth_str : splited) {

                            base_allowed_meths.insert(request::method_parse(meth_str));
                        }

                        std::set<request::method> intersec_result;
                        std::set<request::method>* ac_al_meths = nullptr;

                        switch (type) {

                        case identity_type::anonymous:

                            ac_al_meths = &(this->_anonymous_allowable_meths);
                            break;

                        case identity_type::group:

                            ac_al_meths = &(this->_group_allowable_meths);
                            break;

                        case identity_type::owner:

                            ac_al_meths = &(this->_owner_allowable_meths);
                            break;
                        }

                        std::set_intersection(
                                ac_al_meths->begin(), 
                                ac_al_meths->end(),
                                base_allowed_meths.begin(), 
                                base_allowed_meths.end(),
                                std::inserter(intersec_result, intersec_result.begin())
                            );
                        std::string modified_als_attr_val;

                        for (auto& meth : intersec_result) {

                            modified_als_attr_val += "," + request::method_to_string(meth);
                        }

                        rsp->insert_attribute("allows", modified_als_attr_val);
                        handler(std::move(rsp));
                    }
                );

            } else {

                Requestable::async_request(std::move(req), handler);
            }
        }

        void _do_permission_check(std::unique_ptr<request> req, Requestable::request_handler handler) {

            if (this->_anonymous_allowable_meths.count(req->get_method())) {

                this->_invoke_base_impl(identity_type::group, std::move(req), handler);
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

                    if (this->_group_allowable_meths.count(req->get_method())) {

                        this->_invoke_base_impl(identity_type::group, std::move(req), handler);
                        return;
                    }
                }

                if (usr->name == this->_owner) {

                    if (this->_owner_allowable_meths.count(req->get_method())) {

                        this->_invoke_base_impl(identity_type::group, std::move(req), handler);
                        return;
                    }
                }

                auto rsp = std::make_unique<response>(std::move(req));
                rsp->set_status(403);
                handler(std::move(rsp));
                return;
            }
        }

    public:
        virtual void async_request(std::unique_ptr<request> req, Requestable::request_handler handler) override {

            if (!this->_has_access_controlling) {

                Requestable::async_request(std::move(req), handler);
                return;
            }

            this->_do_permission_check(std::move(req), handler);
        }

        access_control(tinyxml2::XMLElement* config, authentication& auth, Args... args)
            : Requestable(args...), _auth(auth) {

            auto ac = config->FirstChildElement("access_control");

            if (ac) {

                this->_has_access_controlling = true;

            } else {

                this->_has_access_controlling = false;
                return;
            }

            auto owner = ac->Attribute("owner");
            
            if (owner) {

                this->_has_owner = true;
                this->_owner = owner;

            } else {

                this->_has_owner = false;
            }

            auto group = ac->Attribute("group");

            if (group) {

                this->_has_group = true;
                this->_group = group;

            } else {

                this->_has_group = false;
            }

            auto o_als = ac->Attribute("owner_allows");
            auto g_als = ac->Attribute("group_allows");
            auto a_als = ac->Attribute("anon_allows");

            if (a_als) {

                auto a_als_l = utils::split(a_als, ",");

                for (auto& mt : a_als_l) {

                    this->_anonymous_allowable_meths.insert(request::method_parse(mt));
                }
            }

            if (g_als) {

                auto g_als_l = utils::split(g_als, ",");
                this->_group_allowable_meths = this->_anonymous_allowable_meths;

                for (auto& mt : g_als_l) {

                    this->_group_allowable_meths.insert(request::method_parse(mt));
                }
            }

            if (o_als) {

                auto o_als_l = utils::split(o_als, ",");
                this->_owner_allowable_meths = this->_group_allowable_meths;

                for (auto& mt : o_als_l) {

                    this->_owner_allowable_meths.insert(request::method_parse(mt));
                }
            }
        }
    };
}

#endif // ACCESS_CONTROL_HPP