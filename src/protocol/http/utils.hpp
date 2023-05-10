#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <list>
#include <string>

namespace myhttpd::service::http::utils {

    inline std::string ptime_to_http_date(boost::posix_time::ptime time) {

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

    inline std::list<std::string> split(const std::string& str, const std::string& spliter) {

        std::list<std::string> results;
        std::size_t offset = 0;

        while (true) {

            auto begin = offset;
            offset = str.find(spliter, offset);

            if (offset == std::string::npos) {

                auto len = str.size() - begin;
                results.push_back(str.substr(begin, len));
                break;

            } else {

                auto len = offset - begin;
                offset++;
                results.push_back(str.substr(begin, len));
            }
        }

        return results;
    }
}

#endif // HTTP_UTILS_HPP