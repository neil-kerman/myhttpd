#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "message.hpp"

namespace myhttpd::http {

    class request: public message {

    public:
        inline std::string get_version() {

        }

        inline void set_version(std::string version) {

        }

        inline std::string get_method() {

        }

        inline void set_method(std::string method) {

        }

        inline std::string get_url() {

        }

        inline void set_url(std::string url) {

        }

    public:
        virtual network::connection::const_buffer write_to_buffer() {

        }

        virtual std::string& get_title() {

        }

        virtual void set_title(std::string title) {

        }

    public:
        request() {

        }

        request(message&& msg) {

        }

        virtual ~request() = default;
    };
}

#endif // REQUEST_HPP