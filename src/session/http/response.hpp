#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "message.hpp"

namespace myhttpd::http {

    class response: public message {

    private:
        std::string _version = "HTTP/1.1";

        unsigned _status;

    private:
        void _extract_title() {
            auto title = message::get_title();
            std::size_t offset = 0;
            auto size = title.find(' ', 0);
            this->_version = title.substr(0, size);
            offset = size + 1;
            size = title.find(' ', offset) - offset;
            auto status_str = title.substr(offset, size);
            this->_status = std::stoi(status_str);
        }

    public:
        inline std::string get_version() {
            return this->_version;
        }

        inline void set_version(std::string version) {
            this->_version = version;
        }

        inline unsigned get_status() {
            return this->_status;
        }

        inline void set_status(unsigned status) {
            this->_status = status;
        }

    public:
        virtual std::string get_title() {
            return (this->_version + " " + std::to_string(this->_status) + "TODO");
        }

        virtual void set_title(std::string title) {
            message::set_title(title);
            this->_extract_title();
        }

    public:
        response() = default;

        response(message&& msg) 
        : message(std::move(msg)) {
            this->_extract_title();
        }

        virtual ~response() = default;
    };
}

#endif // RESPONSE_HPP