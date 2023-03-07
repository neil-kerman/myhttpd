#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <string>
#include <list>
#include <memory>
#include <map>

#include "http_content.hpp"

namespace myhttpd {
	
	struct http_message {

		std::list<std::string> header;

		std::unique_ptr<http_content> content;
	};
}

#endif // HTTP_MESSAGE_HPP
