#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <map>
#include <memory>

#include "content.hpp"

namespace myhttpd::http {
    
    class message {

    public:
        struct attribute {
            const char* data;
            const std::size_t size;
        };

    private:
        const std::string _raw_header;

        std::map<std::string, attribute> _attributes;

        std::shared_ptr<content> _content;

    private:
        inline std::string _to_lower_case(std::string& str) {
            std::string result = str;
            for (auto& c : result) {
                if (c > 88) c -= 32;
            }
            return result;
        }

        inline void _mark_attributes() {
            auto& raw = this->_raw_header;
            auto title_size = raw.find('\r', 0);
            auto offset = title_size + 2;
            while (raw[offset] != '\r') {
                auto size = raw.find(':', offset) - offset;
                std::string name = raw.substr(offset, size);
                offset += size;
                offset = raw.find_first_not_of(' ', offset);
                size = raw.find('\r', offset) - offset;
                this->_attributes.insert(
                    std::pair<std::string, attribute>(this->_to_lower_case(name), {raw.data() + offset, size})
                );
            }
        }

    public:
        inline const std::string& get_raw_header() {
            return this->_raw_header;
        }

        inline void set_content(std::shared_ptr<content> content) {
            this->_content = content;
        }

        inline std::shared_ptr<content> get_content() {
            return this->_content;
        }

        inline const std::map<std::string, attribute>::iterator find_attribute(std::string name) {
            if (this->_attributes.empty()) {
                this->_mark_attributes();
            }
            return this->_attributes.find(_to_lower_case(name));
        }

        inline const std::map<std::string, attribute>::iterator begin_attribute() {
            if (this->_attributes.empty()) {
                this->_mark_attributes();
            }
            return this->_attributes.begin();
        }

        inline const std::map<std::string, attribute>::iterator end_attribute() {
            if (this->_attributes.empty()) {
                this->_mark_attributes();
            }
            return this->_attributes.end();
        }

    public:
        message(std::string header)
        : _raw_header(header), _content(nullptr) {
            this->_mark_attributes();
        }

        message(std::string header, std::shared_ptr<content> content)
        : _raw_header(header), _content(content) {
            this->_mark_attributes();
        }

        message(message&& msg)
        : _raw_header(std::move(msg._raw_header)), _attributes(std::move(msg._attributes)),
        _content(std::move(msg._content)) {

        }
    };
}

#endif // HTTP_MESSAGE_HPP
