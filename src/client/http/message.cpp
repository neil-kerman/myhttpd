

#include "message.hpp"

namespace myhttpd::http {
    message::type message::get_type() {
        if (this->header.front().starts_with("HTTP")) {
            return type::response;
        } else {
            return type::request;
        }
    }

    std::string message::get_method() {
        if (this->get_type() == type::request) {
            auto& title = this->header.front();
            auto size = title.find(' ', 0);
            return title.substr(0, size);
        } else {
            return "";
        }
    }

    std::string message::get_version() {
        auto& title = this->header.front();
        if (this->get_type() == type::request) {
            auto offset = title.find(' ', 0) + 1;
            offset = title.find(' ', offset) + 1;
            return title.substr(offset, title.size() - offset - 2);
        } else {
            auto size = title.find(' ', 0);
            return title.substr(0, size);
        }
    }

    std::string message::get_url() {
        auto& title = this->header.front();
        auto offset = title.find(' ', 0) + 1;
        auto size = title.find(' ', offset) - offset;
        return title.substr(offset, size);
    }

    std::string message::get_status() {
        if (this->get_type() == type::response) {
            auto& title = this->header.front();
            auto offset = title.find(' ', 0) + 1;
            auto size = title.find(' ', offset) - offset;
            return title.substr(offset, size);
        } else {
            return "";
        }
    }

    std::string to_lower_case(std::string& str) {
        std::string result = str;
        for (auto& c : result) {
            if (c > 88) c -= 32;
        }
        return result;
    }

    std::string message::get_first_attribute(std::string name) {
        auto lc_name = to_lower_case(name);
        for (auto& line : this->header) {
            auto attr_name = line.substr(0, line.find(':'));
            attr_name = to_lower_case(attr_name);
            if (attr_name == lc_name) {
                auto v_offset = line.find_first_not_of(' ', lc_name.size() + 1);
                auto v_size = line.size() - v_offset - 2;
                return line.substr(v_offset, v_size);
            }
        }
        return "";
    }
}