#include <boost/beast/core/detail/base64.hpp>

#include "authentication.hpp"

namespace myhttpd::service::http {


    /*bool base64_encode(const std::string& input, std::string& output) {

        std::size_t len = input.size();
        output.resize(boost::beast::detail::base64::encoded_size(len));
        output.resize(boost::beast::detail::base64::encode(&output[0], input.c_str(), len));
        return true;
    }*/

    void authentication::base64_decode(const std::string& input, std::string& output) {

        std::size_t len = input.size();
        output.resize(boost::beast::detail::base64::decoded_size(len));
        auto result = boost::beast::detail::base64::decode(output.data(), input.data(), len);
        output.resize(result.first);
    }

    std::shared_ptr<user> authentication::authenticate(request& req) {

        auto auth_attr_it = req.find_attribute("authorization");
        bool is_anonymous = true;

        if (auth_attr_it == req.end_attribute()) {

            return nullptr;
        }

        auto value = auth_attr_it->second;
        auto token_begin = value.find("Basic ");

        if (token_begin == std::string::npos) {

            return nullptr;
        }

        token_begin += 6;
        auto token = value.substr(token_begin, value.size() - token_begin);
        std::string decoded_token;
        this->base64_decode(token, decoded_token);

        if (decoded_token.empty()) {

            return nullptr;
        }

        auto pwd_begin = decoded_token.find(':');

        if (pwd_begin == std::string::npos) {

            return nullptr;
        }

        std::string usr = decoded_token.substr(0, pwd_begin);
        pwd_begin += 1;
        std::string pwd = decoded_token.substr(pwd_begin, decoded_token.size() - pwd_begin);
        auto usr_it = this->_users.find(usr);

        if (usr_it == this->_users.end()) {

            return nullptr;
        }

        if (usr_it->second->password != pwd) {

            return nullptr;
        }

        return usr_it->second;
    }

    authentication::authentication(tinyxml2::XMLElement* config) {

        auto usr = config->FirstChildElement("user");

        while (usr) {

            auto user_obj = std::make_shared<user>();
            user_obj->name = usr->Attribute("name");
            user_obj->password = usr->Attribute("password");
            std::string groups = usr->Attribute("groups");
            std::size_t offset = 0;

            while (true) {

                auto begin = offset;
                offset = groups.find(',', offset);

                if (offset == std::string::npos) {
                    
                    auto len = groups.size() - begin;
                    user_obj->groups.insert(groups.substr(begin, len));
                    break;

                } else {

                    auto len = offset - begin;
                    offset++;
                    user_obj->groups.insert(groups.substr(begin, len));
                }
            }

            this->_users.insert(std::pair<std::string, std::shared_ptr<user>>(user_obj->name, user_obj));
            usr = usr->NextSiblingElement();
        }
    }
}

