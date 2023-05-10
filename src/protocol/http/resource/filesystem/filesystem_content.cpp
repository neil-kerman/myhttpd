

#include "filesystem_content.hpp"

namespace myhttpd::service::http {

    std::size_t filesystem_content::_get_size() {

        return this->_range_end - this->_range_begin + 1;
    }

    char* filesystem_content::_get_data() {

        return (char*)this->_region.get_address() + this->_range_begin;
    }

    std::size_t filesystem_content::get_content_langth() {
        return this->_range_end - this->_range_begin + 1;
    }

    void filesystem_content::async_read(read_handler handler, std::size_t offset, std::size_t size) {

        handler(content::success, { this->_get_data(), this->_get_size() });
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