#include <exception>

namespace myhttpd {
    class config_exception: public std::exception {
    public: 
        config_exception(const char *message, int line_number);
    };
}