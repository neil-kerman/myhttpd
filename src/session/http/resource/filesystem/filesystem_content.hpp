#ifndef FILESYSTEM_CONTENT_HPP
#define FILESYSTEM_CONTENT_HPP

#include <string>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "../../transceiver/content.hpp"

using namespace boost::interprocess;

namespace myhttpd::session::http {

    class filesystem_content: public content {

    private:
        boost::interprocess::file_mapping _file;

        boost::interprocess::mapped_region _region;

    public:
        virtual std::size_t get_size();

        virtual const void* get_data();

        virtual void async_wait_ready(wait_handler handler);

    public:
        filesystem_content() = delete;

        filesystem_content(std::string path);

        virtual ~filesystem_content() = default;
    };
}

#endif // FILESYSTEM_CONTENT_HPP
