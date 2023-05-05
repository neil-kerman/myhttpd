#ifndef FILESYSTEM_CONTENT_HPP
#define FILESYSTEM_CONTENT_HPP

#include <string>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "../../transceiver/content.hpp"

using namespace boost::interprocess;

namespace myhttpd::protocol::http {

    class filesystem_content: public content {

    private:
        boost::interprocess::file_mapping _file;

        boost::interprocess::mapped_region _region;

        std::size_t _range_begin;

        std::size_t _range_end;

    public:
        std::size_t _get_size();

        char* _get_data();

    public:
        void set_range_begin(std::size_t index);

        void set_range_end(std::size_t index);

    public:
        virtual std::size_t get_content_langth();

        virtual void async_read(read_handler handler, std::size_t offset = 0, std::size_t size = 0);

    public:
        filesystem_content() = delete;

        filesystem_content(std::string path);

        virtual ~filesystem_content() = default;
    };
}

#endif // FILESYSTEM_CONTENT_HPP
