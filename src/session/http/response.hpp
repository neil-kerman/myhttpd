#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "message.hpp"

namespace myhttpd::http {

    class response: public message {

    private:
        std::string _version = "HTTP/1.1";

        unsigned _status;

    private:
        static const std::string& _get_status_meaning(unsigned status) {

            switch (status) {
            case 100: return "continue";
            case 101: return "switching protocols";
            case 200: return "ok";
            case 201: return "created";
            case 202: return "accepted";
            case 203: return "non-authoritative information";
            case 204: return "no content";
            case 205: return "reset content";
            case 206: return "partial content";
            case 300: return "multiple choices";
            case 301: return "moved permanently";
            case 302: return "found";
            case 303: return "see other";
            case 304: return "not modified";
            case 305: return "use proxy";
            case 306: return "*";
            case 307: return "temporary redirect";
            case 400: return "bad request";
            case 401: return "unauthorized";
            case 402: return "payment required";
            case 403: return "forbidden";
            case 404: return "not found";
            case 405: return "method not allowed";
            case 406: return "not acceptable";
            case 407: return "proxy authentication required";
            case 408: return "request timeout";
            case 409: return "conflict";
            case 410: return "gone";
            case 411: return "length required";
            case 412: return "precondition failed";
            case 413: return "request entity too large";
            case 414: return "request-uri too long";
            case 415: return "unsupported media type";
            case 416: return "requested range not satisfiable";
            case 417: return "expectation failed";
            case 500: return "internal server error";
            case 501: return "not implemented";
            case 502: return "bad gateway";
            case 503: return "service unavailable";
            case 504: return "gateway timeout";
            case 505: return "http version not supported";
            default: return "*";
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
            title.append(version).append(" ").append(std::to_string(this->_status)).append(status_meaning);
            return title;
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