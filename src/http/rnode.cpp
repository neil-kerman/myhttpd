#include "rnode.hpp"

namespace myhttpd {
    rnode::rnode(std::string vpath)
    : _vpath(vpath) {}

    rnode::~rnode() {}

    std::string rnode::get_vpath() {
        return this->_vpath;
    }    
}



