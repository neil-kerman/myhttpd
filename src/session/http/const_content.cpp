#include <string>

#include "const_content.hpp"

namespace myhttpd::http {

    const_content const_content::from_c_string(const char* str) {
        return const_content(str, std::strlen(str));
    }

    std::size_t const_content::get_size() {
        return this->_size;
    }

    const void* const_content::get_data() {
        return nullptr;
    }

    void const_content::async_wait_ready(wait_handler handler) {
        asio_error_code error;
        handler(error, { this->_data, this->_size });
    }

    const_content::const_content(const void* data, std::size_t size) 
    : _data(data), _size(size) {
    }
}
