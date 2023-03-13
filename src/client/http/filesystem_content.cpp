

#include "filesystem_content.hpp"

namespace myhttpd::http {

    void filesystem_content::async_wait_ready(wait_handler handler) {
        handler(error_code::successful, this->_region.get_address(), this->_region.get_size());
    }

    filesystem_content::filesystem_content(std::string path)
    : _file(file_mapping(path.c_str(), boost::interprocess::mode_t::read_only)),
    _region(mapped_region(this->_file, boost::interprocess::mode_t::read_only)) {}

    filesystem_content::~filesystem_content() {
        
    }
}
