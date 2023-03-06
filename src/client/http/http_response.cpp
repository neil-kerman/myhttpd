

#include "http_response.hpp"

namespace myhttpd {

	int myhttpd::http_response::get_status() {
		return 200;
	}

	void myhttpd::http_response::set_status(int s) {
		this->_status = s;
	}

	std::string myhttpd::http_response::get_attribute(std::string name) {
		return std::string();
	}

	void myhttpd::http_response::set_attribute(std::string name, std::string value) {

	}

	std::shared_ptr<http_content>& myhttpd::http_response::get_content() {
		// TODO: insert return statement here
	}

	void myhttpd::http_response::set_contnet(std::shared_ptr<http_content> content) {

	}

	std::string http_response::get_header() {
		return std::string();
	}
}

