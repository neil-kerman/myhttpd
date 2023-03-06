#include <string>
#include <map>

#include "http_content.hpp"

namespace myhttpd {
	
	class http_message {

	public:
		enum type {
			request,
			response,
		};

		enum version {
			v1_0,
			v1_1,
		};

	protected:
		std::string _title;

		std::map<std::string, std::string> _attributes;

		std::unique_ptr<http_contnet> _content;

	public:

	};
}