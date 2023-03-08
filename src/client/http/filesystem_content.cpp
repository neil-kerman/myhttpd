

#include "filesystem_content.hpp"

namespace myhttpd::http {
    
    const char* filesystem_content::get_ptr() {
        return this->get_ptr();
    }

    std::size_t filesystem_content::get_size() {
        return this->get_size();
    }

    filesystem_content::filesystem_content(std::string path)
    : _file(file_mapping(path.c_str(), boost::interprocess::mode_t::read_only)),
    _region(mapped_region(this->_file, boost::interprocess::mode_t::read_only)) {}

    filesystem_content::~filesystem_content() {
        
    }
}
