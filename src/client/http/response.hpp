#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <memory>

#include "message.hpp"

namespace myhttpd::http {
    
    class response {

    private:
        std::string _status;

        std::string _version;

        std::map<std::string, std::string> _attributes;

        std::shared_ptr<content> _content;

    public:
        inline void set_version(std::string &version) {
            this->_version = version;
        }

        inline void set_status(std::string &status) {
            this->_status = status;
        }

        inline void insert_attribute(std::string &name, std::string &value) {
            this->_attributes.insert(std::pair<std::string, std::string>(name, value));
        }

        inline std::size_t count_attribute(std::string& name) {
            return this->_attributes.count(name);
        }

        inline void set_content(std::shared_ptr<content> content) {
            this->_content = content;
        }

        inline std::unique_ptr<message> to_message() {
            auto msg = std::make_unique<message>();
            std::string line;
            line.reserve(this->_version.size() + this->_status.size() + 2);
            line.append(this->_version).append(" ");
            line.append(this->_status).append("\r\n");
            msg->header.push_back(line);
            for (auto& attr : this->_attributes) {
                line.clear();
                line.reserve(attr.first.size() + 2 + attr.second.size() + 2);
                line.append(attr.first).append(": ").append(attr.second).append("\r\n");
                msg->header.push_back(line);
            }
            msg->content = this->_content;
            return msg;
        }

    public:
        response() {}
    };
}

#endif // RESPONSE_HPP