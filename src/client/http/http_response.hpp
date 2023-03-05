#include <map>
#include <string>

namespace myhttpd {

    struct http_response {

    private:
        int status;

        std::map<std::string, std::string> _attributes;

    public:
        int get_status();

        void set_status(int s);

        std::string get_attribute(std::string name);

        void set_attribute(std::string name, std::string value);

    };
}