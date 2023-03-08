#ifndef FILESYSTEM_CONTENT_HPP
#define FILESYSTEM_CONTENT_HPP

#include <string>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "content.hpp"

using namespace boost::interprocess;

namespace myhttpd::http {
    class filesystem_content: public content {

    private:
        boost::interprocess::file_mapping _file;

        boost::interprocess::mapped_region _region;

    public:
        filesystem_content(std::string path);

        virtual ~filesystem_content();

        virtual const char* get_ptr();
        
        virtual std::size_t get_size();
    };
}

#endif // FILESYSTEM_CONTENT_HPP
