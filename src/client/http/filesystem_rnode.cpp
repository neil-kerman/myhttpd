#include <boost/filesystem.hpp>

#include "filesystem_content.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd::http {

	bool filesystem_rnode::exists(std::string url) {
		std::string abs_path = this->_path + url;
		return boost::filesystem::exists(abs_path);
	}

	void filesystem_rnode::async_request(std::string url, std::unique_ptr<request> req, request_handler handler) {
		
	}

	filesystem_rnode::filesystem_rnode(std::string path) 
	: _path(path) {}

	filesystem_rnode::~filesystem_rnode() {
	}
}