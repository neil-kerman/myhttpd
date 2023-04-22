

#include "filesystem_content.hpp"

namespace myhttpd::session::http {

    std::size_t filesystem_content::get_size() {

        return this->_range_end - this->_range_begin + 1;
    }

    const void* filesystem_content::get_data() {

        return (void*)((std::size_t)(this->_region.get_address()) + this->_range_begin);
    }

    void filesystem_content::async_wait_ready(wait_handler handler) {

        asio_error_code error;
        handler(error, { this->get_data(), this->get_size() });
    }

    void filesystem_content::set_range_begin(std::size_t index) {

        this->_range_begin = index;
    }

    void filesystem_content::set_range_end(std::size_t index) {

        this->_range_end = index;
    }

    filesystem_content::filesystem_content(std::string path): 
        _file(file_mapping(path.c_str(), boost::interprocess::mode_t::read_only)),
        _region(mapped_region(this->_file, boost::interprocess::mode_t::read_only)) {

        this->_range_begin = 0;
        this->_range_end = this->_region.get_size() - 1;
    }
}