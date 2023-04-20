

#include "filesystem_content.hpp"

namespace myhttpd::session::http {

    std::size_t filesystem_content::get_size() {

        return this->_region.get_size();
    }

    const void* filesystem_content::get_data() {

        return nullptr;
    }

    void filesystem_content::async_wait_ready(wait_handler handler) {

        asio_error_code error;
        handler(error, { this->_region.get_address(), this->_region.get_size() });
    }

    filesystem_content::filesystem_content(std::string path): 
        _file(file_mapping(path.c_str(), boost::interprocess::mode_t::read_only)),
        _region(mapped_region(this->_file, boost::interprocess::mode_t::read_only)) {

    }
}