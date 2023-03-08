#include <boost/filesystem.hpp>

#include "filesystem_content.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd::http {

	filesystem_rnode::filesystem_rnode(std::string path) 
	: _path(path) {}

	void filesystem_rnode::async_request(std::string url, std::unique_ptr<request> req, request_handler handler) {
		std::string absolute_path;
		
	}
}