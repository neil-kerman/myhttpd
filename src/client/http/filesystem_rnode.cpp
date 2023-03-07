#include <boost/filesystem.hpp>

#include "http_filesystem_content.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd {

	filesystem_rnode::filesystem_rnode(std::string path) 
	: _path(path) {}

	void filesystem_rnode::async_request(std::string url, std::unique_ptr<http_request> req, request_handler handler) {
		std::string absolute_path;
		
	}
}