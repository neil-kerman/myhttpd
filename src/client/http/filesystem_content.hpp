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
        virtual void async_wait_ready(wait_handler handler);

    public:
        filesystem_content(std::string path);

        virtual ~filesystem_content();
    };
}

#endif // FILESYSTEM_CONTENT_HPP
