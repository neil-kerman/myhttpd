#include <string>
#include <map>
#include <vector>
#include <boost/asio/buffer.hpp>

class http_header {
private:
    std::map<std::string, std::string> _fields;
public:
    static http_header parse(boost::asio::const_buffer buf);
    std::string get_field_value(std::string field_name);
    std::string set_field_value(std::string field_name, std::string value);
    std::vector<char> get_data();
};