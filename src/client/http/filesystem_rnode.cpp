#include <boost/filesystem.hpp>

#include "http_filesystem_content.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd {

	filesystem_rnode::filesystem_rnode(std::string path) 
	: _path(path) {}

	void filesystem_rnode::async_request(std::string url, std::unique_ptr<http_request> req, request_handler handler) {
		std::string absolute_path;
		absolute_path.reserve(this->_path.size() + url.size());
		absolute_path.append(this->_path).append(url);
		auto rsp = std::make_unique<http_response>();
		if (boost::filesystem::exists(absolute_path)) {
			rsp->set_status(200);
			rsp->set_contnet(std::make_shared<http_filesystem_content>(absolute_path));
		}
		else {
			rsp->set_status(404);
		}
		handler(std::move(rsp));
	}
}