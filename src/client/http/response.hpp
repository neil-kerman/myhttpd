#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <memory>

#include "message.hpp"

namespace myhttpd::http {
    
    class response {

    private:
        std::map<std::string, std::string> _attributes;

        std::shared_ptr<content> _content;

    public:
        inline void set_version(std::string &version) {

        }

        inline void set_status(std::string &status) {

        }

        inline void insert_attribute(std::string &name, std::string &value) {

        }

        inline std::size_t count_attribute(std::string& name) {

        }

        inline void set_content(std::shared_ptr<content> content) {

        }

        inline std::unique_ptr<message> to_message() {

        }

    public:
        response() {}
    };
}

#endif // RESPONSE_HPP