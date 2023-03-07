#include <string>
#include <list>
#include <memory>
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

		std::map<std::string, std::list<std::string>> _attributes;

		std::unique_ptr<http_content> _content;

	public:
		void set_title(std::string);

		std::string get_title();

		void set_attrubute(std::string name, std::list<std::string>);

		std::list<std::string>& get_attribute(std::string name);

		bool has_attribute(std::string name);
	};
}