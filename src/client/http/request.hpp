#include <memory>
#include <string>
#include <map>

#include "message.hpp"

namespace myhttpd::http {

    class request {

    private:
        std::unique_ptr<message> _raw;

        std::string _method;

        std::string _url;

        std::string _version;

        std::map<std::string, std::string> _attributes;

    private:
        inline std::string _to_lower_case(std::string& str) {
            std::string result = str;
            for (auto& c : result) {
                if (c > 88) c -= 32;
            }
            return result;
        }

        inline void _extract_title() {
            auto& title = this->_raw->header.front();
            auto offset1 = title.find(' ', 0);
            this->_method = title.substr(0, offset1);
            auto offset2 = title.find(' ', offset1 + 1);
            this->_url = title.substr(offset1 + 1, offset2 - (offset1 + 1));
            auto offset3 = title.find(' ', offset2 + 1);
            this->_version = title.substr(offset2 + 1, offset3 - (offset2 + 1));
        }

        inline void _extract_attributes() {
            auto it = this->_raw->header.begin();
            for (it++; it != this->_raw->header.end(); it++) {
                std::string& line = *it;
                auto attr_name = line.substr(0, line.find(':'));
                attr_name = _to_lower_case(attr_name);
                auto v_offset = line.find_first_not_of(' ', attr_name.size() + 1);
                auto v_size = line.size() - v_offset - 2;
                auto attr_value = line.substr(v_offset, v_size);
                this->_attributes.insert(std::pair<std::string, std::string>(_to_lower_case(attr_name), attr_value));
            }
        }

    public:
        inline std::string get_method() {
            return this->_method;
        }

        inline std::string get_url() {
            return this->_url;
        }

        inline std::string get_version() {
            return this->_version;
        }

        inline std::map<std::string, std::string>::iterator get_attribute(std::string name) {
            return this->_attributes.find(_to_lower_case(name));
        }

    public:
        request(std::unique_ptr<message> msg)
        : _raw(std::move(msg)) {
            this->_extract_title();
            this->_extract_attributes();
        }
    };
}