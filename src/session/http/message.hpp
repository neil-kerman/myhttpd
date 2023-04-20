#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

#include "network/connection.hpp"
#include "content.hpp"

namespace myhttpd::session::http {
    
    class message {

    private:
        std::string _title;

        std::multimap<std::string, std::string> _attributes;

        std::shared_ptr<content> _content = nullptr;

    private:
        static inline std::string _to_lower_case(const std::string str) {

            std::string result = str;

            for (auto& c : result) {

                if (c >= 64 && c <= 90) {

                    c += 32;
                }
            }

            return result;
        }

        inline void _extract(const std::string raw_header) {

            auto& raw = raw_header;
            auto title_size = raw.find('\r', 0);
            this->set_title(std::string(raw_header.substr(0, title_size)));
            auto offset = title_size + 2;

            while (raw[offset] != '\r') {

                auto name_size = raw.find(':', offset) - offset;
                std::string name = this->_to_lower_case(raw.substr(offset, name_size));
                offset += name_size + 1;
                offset = raw.find_first_not_of(' ', offset);
                auto value_size = raw.find('\r', offset) - offset;
                std::string value(raw.data() + offset, value_size);
                this->insert_attribute(std::move(name), std::move(value));
                offset += value_size + 2;
            }
        }

    public:
        static inline bool assert_attribute(std::multimap<std::string, std::string>::iterator attr, std::string value) {

            return (_to_lower_case(attr->second) == _to_lower_case(value));
        }

        inline void insert_attribute(std::string name, std::string value) {

            this->_attributes.insert(std::pair<std::string, std::string>(this->_to_lower_case(name), value));
        }

        inline std::multimap<std::string, std::string>::iterator find_attribute(std::string name) {

            return this->_attributes.find(this->_to_lower_case(name));
        }

        inline std::multimap<std::string, std::string>::iterator begin_attribute() {

            return this->_attributes.begin();
        }

        inline std::multimap<std::string, std::string>::iterator end_attribute() {

            return this->_attributes.end();
        }

        inline void erase_attribute(std::multimap<std::string, std::string>::iterator att) {

            this->_attributes.erase(att);
        }

        inline bool contains_attribute(std::string name) {

            return this->_attributes.contains(this->_to_lower_case(name));
        }

        inline void set_content(std::shared_ptr<content> cont) {

            this->_content = cont;
        }

        inline std::shared_ptr<content> get_content() {

            return this->_content;
        }

        inline bool has_content() {

            return (this->_content != nullptr);
        }

    public:
        virtual std::string get_title() {

            return this->_title;
        }

        virtual void set_title(std::string title) {

            this->_title = title;
        }

    public:
        message(): 
            _content(nullptr) {

        }

        message(const std::string header): 
            _content(nullptr) {

            this->_extract(header);
        }

        message(const std::string header, std::shared_ptr<content> content): 
            _content(content) {

            this->_extract(header);
        }
        
        message(message&& msg) noexcept: 
            _title(std::move(msg._title)), 
            _attributes(std::move(msg._attributes)), 
            _content(msg._content) {

        } 

        virtual ~message() = default;
    };
}

#endif // HTTP_MESSAGE_HPP
