

#include "http_content.hpp"

namespace myhttpd {
    class http_filesystem_content {

    public:
        virtual ~http_filesystem_content();

        virtual const char* get_ptr();
        
        virtual std::size_t get_size();
    };
}