#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <list>
#include <memory>

#include "content.hpp"

namespace myhttpd::http {
	
	struct message {

		std::list<std::string> header;

		std::shared_ptr<content> content;

	public:
		enum type {
			request,
			response,
		};

	public:
        inline message::type get_type() {
            if (this->header.front().starts_with("HTTP")) {
                return type::response;
            } else {
                return type::request;
            }
        }

        inline std::string get_version() {
            auto& title = this->header.front();
            if (this->get_type() == type::request) {
                auto offset = title.find(' ', 0) + 1;
                offset = title.find(' ', offset) + 1;
                return title.substr(offset, title.size() - offset - 2);
            } else {
                auto size = title.find(' ', 0);
                return title.substr(0, size);
            }
        }
	};
}

#endif // HTTP_MESSAGE_HPP
