

#include "utils.hpp"

namespace myhttpd::protocol::http::utils {

    std::string ptime_to_http_date(boost::posix_time::ptime time) {

        auto date = time.date();
        std::string day_of_week_str = date.day_of_week().as_short_string();
        auto days_of_month = date.day().as_number();
        std::string month_str = date.month().as_short_string();
        auto years = date.year();
        auto hours = time.time_of_day().hours();
        auto minuts = time.time_of_day().minutes();
        auto seconds = time.time_of_day().seconds();
        return boost::str(boost::format("%s, %d %s %d %02d:%02d:%02d GMT")
            % day_of_week_str
            % days_of_month
            % month_str
            % years
            % hours
            % minuts
            % seconds
        );
    }
}