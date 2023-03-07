#include "http_message.hpp"
#include "http_message.hpp"


#include "http_message.hpp"

namespace myhttpd {

	void http_message::set_title(std::string title) {
		this->_title = title;
	}

	std::string http_message::get_title() {
		return this->_title;
	}

	void http_message::set_attrubute(std::string name, std::list<std::string> values) {
		this->_attributes.insert(std::pair<std::string, std::list<std::string>>(name, values));
	}

	std::list<std::string> http_message::get_attribute(std::string name) {
		return this->_attributes[name];
	}

	bool myhttpd::http_message::has_attribute(std::string name) {
		return this->_attributes.count(name) != 0;
	}
}