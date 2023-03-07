#ifndef HTTP_FILESYSTEM_CONTENT_HPP
#define HTTP_FILESYSTEM_CONTENT_HPP

#include <string>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "http_content.hpp"

using namespace boost::interprocess;

namespace myhttpd {
    class http_filesystem_content: public http_content {

    private:
        boost::interprocess::file_mapping _file;

        boost::interprocess::mapped_region _region;

    public:
        http_filesystem_content(std::string path);

        virtual ~http_filesystem_content();

        virtual const char* get_ptr();
        
        virtual std::size_t get_size();
    };
}

#endif // HTTP_FILESYSTEM_CONTENT_HPP
