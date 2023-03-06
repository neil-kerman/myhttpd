#include "http_request.hpp"

namespace myhttpd {

    std::string http_request::get_attribute(std::string name) const {
        return std::string();
    }

    std::string http_request::get_method() const {
        return std::string();
    }

    std::string http_request::get_url() const {
        return std::string();
    }

    http_request::http_request(std::string &&header)
    : _header(std::move(header)) {
        
    }
    
}
