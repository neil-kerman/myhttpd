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

	public:
		enum type {
			request,
			response,
		};

	public:
		type get_type();

		std::string get_method();

		std::string get_version();

		std::string get_url();

		std::string get_status();

		std::string get_first_attribute(std::string name);
	};
}

#endif // HTTP_MESSAGE_HPP
