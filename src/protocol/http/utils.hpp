#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include <boost/date_time.hpp>
#include <boost/format.hpp>

namespace myhttpd::service::http::utils {

    std::string ptime_to_http_date(boost::posix_time::ptime time);
}

#endif // HTTP_UTILS_HPP