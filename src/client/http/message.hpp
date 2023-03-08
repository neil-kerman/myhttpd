#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <list>
#include <memory>

#include "content.hpp"

namespace myhttpd::http {
	
	struct message {

		std::list<std::string> header;

		std::unique_ptr<content> content;
	};
}

#endif // HTTP_MESSAGE_HPP
