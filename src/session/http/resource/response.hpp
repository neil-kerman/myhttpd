#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../transceiver/message.hpp"

namespace myhttpd::session::http {

    class response: public message {

    private:
        std::string _version = "HTTP/1.1";

        unsigned _status;

    private:
        static const std::string& _get_status_meaning(unsigned status) {

            static const std::string status_100_meaning = "Continue";
            static const std::string status_101_meaning = "Switching Protocols";
            static const std::string status_200_meaning = "OK";
            static const std::string status_201_meaning = "Created";
            static const std::string status_202_meaning = "Accepted";
            static const std::string status_203_meaning = "Non-Authoritative Information";
            static const std::string status_204_meaning = "No Content";
            static const std::string status_205_meaning = "Reset Content";
            static const std::string status_206_meaning = "Partial Content";
            static const std::string status_300_meaning = "Multiple Choices";
            static const std::string status_301_meaning = "Moved Permanently";
            static const std::string status_302_meaning = "Found";
            static const std::string status_303_meaning = "See Other";
            static const std::string status_304_meaning = "Not Modified";
            static const std::string status_305_meaning = "Use Proxy";
            static const std::string status_306_meaning = "*";
            static const std::string status_307_meaning = "Temporary Redirect";
            static const std::string status_400_meaning = "Bad Request";
            static const std::string status_401_meaning = "Unauthorized";
            static const std::string status_402_meaning = "Payment Required";
            static const std::string status_403_meaning = "Forbidden";
            static const std::string status_404_meaning = "Not Found";
            static const std::string status_405_meaning = "Method Not Allowed";
            static const std::string status_406_meaning = "Not Acceptable";
            static const std::string status_407_meaning = "Proxy Authentication Required";
            static const std::string status_408_meaning = "Request Timeout";
            static const std::string status_409_meaning = "Conflict";
            static const std::string status_410_meaning = "Gone";
            static const std::string status_411_meaning = "Length Required";
            static const std::string status_412_meaning = "Precondition Failed";
            static const std::string status_413_meaning = "Request Entity Too Large";
            static const std::string status_414_meaning = "Request-Uri Too Long";
            static const std::string status_415_meaning = "Unsupported Media Type";
            static const std::string status_416_meaning = "Requested Range Not Satisfiable";
            static const std::string status_417_meaning = "Expectation Failed";
            static const std::string status_500_meaning = "Internal Server Error";
            static const std::string status_501_meaning = "Not Implemented";
            static const std::string status_502_meaning = "Bad Gateway";
            static const std::string status_503_meaning = "Service Unavailable";
            static const std::string status_504_meaning = "Gateway Timeout";
            static const std::string status_505_meaning = "Http Version Not Supported";
            static const std::string status_undefined_meaning = "*";

            switch (status) {

            case 100: return status_100_meaning;
            case 101: return status_101_meaning;
            case 200: return status_200_meaning;
            case 201: return status_201_meaning;
            case 202: return status_202_meaning;
            case 203: return status_203_meaning;
            case 204: return status_204_meaning;
            case 205: return status_205_meaning;
            case 206: return status_206_meaning;
            case 300: return status_300_meaning;
            case 301: return status_301_meaning;
            case 302: return status_302_meaning;
            case 303: return status_303_meaning;
            case 304: return status_304_meaning;
            case 305: return status_305_meaning;
            case 306: return status_306_meaning;
            case 307: return status_307_meaning;
            case 400: return status_400_meaning;
            case 401: return status_401_meaning;
            case 402: return status_402_meaning;
            case 403: return status_403_meaning;
            case 404: return status_404_meaning;
            case 405: return status_405_meaning;
            case 406: return status_406_meaning;
            case 407: return status_407_meaning;
            case 408: return status_408_meaning;
            case 409: return status_409_meaning;
            case 410: return status_410_meaning;
            case 411: return status_411_meaning;
            case 412: return status_412_meaning;
            case 413: return status_413_meaning;
            case 414: return status_414_meaning;
            case 415: return status_415_meaning;
            case 416: return status_416_meaning;
            case 417: return status_417_meaning;
            case 500: return status_500_meaning;
            case 501: return status_501_meaning;
            case 502: return status_502_meaning;
            case 503: return status_503_meaning;
            case 504: return status_504_meaning;
            case 505: return status_505_meaning;
            default: return status_undefined_meaning;
            }
        }

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
        inline std::string& get_version() {

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

            auto& status_meaning = this->_get_status_meaning(this->_status);
            auto& version = this->get_version();
            std::string title;
            title.reserve(version.size() + 5 + status_meaning.size());
            title.append(version).append(" ").append(std::to_string(this->_status)).append(" ").append(status_meaning);
            return title;
        }

        virtual void set_title(std::string title) {

            message::set_title(title);
            this->_extract_title();
        }

    public:
        response() = default;

        response(message&& msg): 
            message(std::move(msg)) {

            this->_extract_title();
        }

        virtual ~response() = default;
    };
}

#endif // RESPONSE_HPP