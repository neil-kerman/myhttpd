#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "message.hpp"

namespace myhttpd::http {

    class response: public message {

    private:
        std::string _version;

        unsigned _status;

    public:
        inline std::string get_version() {

        }

        inline void set_version(std::string version) {

        }

        inline unsigned get_status() {

        }

        inline void set_status(unsigned status) {

        }

    public:
        virtual std::string& get_title() {

        }

        virtual void set_title(std::string title) {

        }

        virtual network::connection::const_buffer write_to_buffer() {

        }

    public:
        response() {

        }

        response(message&& msg) {

        }

        virtual ~message() = default;
    };
}

#endif // RESPONSE_HPP