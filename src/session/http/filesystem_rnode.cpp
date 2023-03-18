#include <boost/filesystem.hpp>

#include "filesystem_content.hpp"
#include "filesystem_rnode.hpp"
#include "const/error_page.hpp"
#include "const_content.hpp"

namespace myhttpd::http {

	bool filesystem_rnode::_exists(std::string url) {
		std::string abs_path = this->_path + url;
		return boost::filesystem::exists(abs_path) && boost::filesystem::is_regular_file(abs_path);
	}

	void filesystem_rnode::async_request(std::string url, std::unique_ptr<message> req, request_handler handler) {
		auto rsp = std::make_shared<message>();
		if (this->_exists(url)) {
			rsp->set_title("HTTP/1.1 200 Ok");
			rsp->set_content(std::make_shared<filesystem_content>(this->_path + url));
		} else {
			rsp->set_title("HTTP/1.1 404 Not Found");
			rsp->set_content(std::make_shared<const_content>(page_404_html, (std::size_t)page_404_html_size));
			rsp->insert_attribute("content-type", "text/html");
		}
		handler(rsp);
	}

	filesystem_rnode::filesystem_rnode(std::string path) 
	: _path(path) {}
}