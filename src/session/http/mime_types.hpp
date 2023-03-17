#ifndef MIME_TYPES_HPP
#define MIME_TYPES_HPP

#include <unordered_map>
#include <memory>
#include <string>

namespace myhttpd::http {

    class mime_types {

    public:
        static const std::unordered_map<std::string, std::string>& get_table();
    };
}
#endif // HTTP_MIME_TYPES_HPP
