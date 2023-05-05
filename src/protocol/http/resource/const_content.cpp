#include <string>

#include "const_content.hpp"

namespace myhttpd::protocol::http {

    const_content const_content::from_c_string(const char* str) {

        return const_content(str, std::strlen(str));
    }

    std::size_t const_content::get_content_langth() {

        return this->_size;
    }

    void const_content::async_read(read_handler handler, std::size_t offset, std::size_t size) {

        handler(content::success, { this->_data, this->_size });
        return;
    }

    const_content::const_content(const char* data, std::size_t size): 
        _data(data), _size(size) {

        return;
    }
}
