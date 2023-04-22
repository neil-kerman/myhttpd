#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "authentication.hpp"

namespace myhttpd::session::http {

    std::string authentication::_to_based64_string(const std::string str) {
        namespace its = boost::archive::iterators;
        typedef its::binary_from_base64<
            its::transform_width<
                std::string::const_iterator,
                6,
                sizeof(std::string::value_type) * 8
            >
        > based64_string;
        std::stringstream stream;
        std::copy(based64_string(str.begin()), based64_string(str.end()), std::ostream_iterator<char>(stream));
        return stream.view().data();
    }
}

