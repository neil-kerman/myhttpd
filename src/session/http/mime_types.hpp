#ifndef MIME_TYPES_HPP
#define MIME_TYPES_HPP

#include <unordered_map>
#include <memory>
#include <string>

namespace myhttpd::http {

    extern static const std::unordered_map<std::string, std::string> mime_types;
}
#endif // HTTP_MIME_TYPES_HPP
