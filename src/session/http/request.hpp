#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "message.hpp"

namespace myhttpd::http {

    class request: public message {

    private:
        std::string _method;

        std::string _url;

        std::string _version;

    private:
        void _extract_title() {
            auto title = message::get_title();
            std::size_t offset = 0;
            auto size = title.find(' ', 0);
            this->_method = title.substr(0, size);
            offset = size + 1;
            size = title.find(' ', offset) - offset;
            this->_url = title.substr(offset, size);
            offset += size + 1;
            size = title.find('\r', offset) - offset;
            this->_version = title.substr(offset, size);
        }

    public:
        inline std::string get_version() {
            return this->_version;
        }

        inline void set_version(std::string version) {
            this->_version = version;
        }

        inline std::string get_method() {
            return this->_method;
        }

        inline void set_method(std::string method) {
            this->_method = method;
        }

        inline std::string get_url() {
            return this->_url;
        }

        inline void set_url(std::string url) {
            this->_url = url;
        }

    public:
        virtual std::string get_title() {
            return (this->_method + " " + this->_url + " " + this->_version);
        }

        virtual void set_title(std::string title) {
            message::set_title(title);
            this->_extract_title();
        }

    public:
        request() = default;

        request(message&& msg)
        : message(std::move(msg)){
            this->_extract_title();
        }

        virtual ~request() = default;
    };
}

#endif // REQUEST_HPP