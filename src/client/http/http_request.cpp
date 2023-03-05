#include "http_request.hpp"

namespace myhttpd {

    std::string http_request::get_attribute(std::string name) const {
        return "";
    }

    http_request::http_request(std::string &&header)
    : _header(std::move(header)) {
        
    }
    
}
