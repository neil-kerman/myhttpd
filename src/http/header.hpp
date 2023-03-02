#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <string>
#include <map>
#include <vector>

namespace myhttpd {
    class header {
    private:
        std::map<std::string, std::string> _fields;
    public:
        static header parse(std::vector<char> data);
        std::string get_field_value(std::string field_name);
        std::string set_field_value(std::string field_name, std::string value);
        std::vector<char>& get_data();
    };    
}

#endif // HTTP_HEADER_HPP
