#include <string>

namespace myhttpd {

    struct http_request {

    private:

    public:
        std::string _header;

        http_request(std::string &&header);
        
        std::string get_attribute(std::string name) const;

        std::string get_method() const;

        std::string get_url() const;
    };
}