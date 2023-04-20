#include <boost/filesystem.hpp>

#include "filesystem_content.hpp"
#include "filesystem_rnode.hpp"
#include "const/error_page.hpp"
#include "const_content.hpp"

namespace myhttpd::session::http {

	bool filesystem_rnode::_exists(std::string url) {

		std::string abs_path = this->_path + url;
		return boost::filesystem::exists(abs_path) && boost::filesystem::is_regular_file(abs_path);
	}

	void filesystem_rnode::async_request(std::shared_ptr<request> req, request_handler handler) {

		auto rsp = std::make_shared<response>();

		if (this->_exists(req->get_url())) {

			rsp->set_status(200);
			rsp->set_content(std::make_shared<filesystem_content>(this->_path + req->get_url()));

		} else {

			rsp->set_status(404);
		}

		handler(rsp);
	}

	filesystem_rnode::filesystem_rnode(std::string path): 
		_path(path) {

	}
}