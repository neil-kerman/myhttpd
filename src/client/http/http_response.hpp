#include <map>
#include <string>
#include <memory>

#include "http_content.hpp"

namespace myhttpd {

    struct http_response {

    private:
        int _status;

        std::map<std::string, std::string> _attributes;

    public:
        int get_status();

        void set_status(int s);

        std::string get_attribute(std::string name);

        void set_attribute(std::string name, std::string value);

        std::shared_ptr<http_content>& get_content();

        void set_contnet(std::shared_ptr<http_content> content);

        std::string get_header();
    };
}