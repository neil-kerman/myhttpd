

#include "http_filesystem_content.hpp"

namespace myhttpd {
    
    const char* http_filesystem_content::get_ptr() {
        return this->get_ptr();
    }

    std::size_t http_filesystem_content::get_size() {
        return this->get_size();
    }

    http_filesystem_content::http_filesystem_content(std::string path)
    : _file(file_mapping(path.c_str(), boost::interprocess::mode_t::read_only)),
    _region(mapped_region(this->_file, boost::interprocess::mode_t::read_only)) {}

    http_filesystem_content::~http_filesystem_content() {
        
    }
}
