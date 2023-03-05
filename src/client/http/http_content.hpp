#include <cstddef>

namespace myhttpd {
    class http_content {

    public:
        virtual ~http_content() {}

        virtual const char* get_ptr() = 0;

        virtual std::size_t get_size() = 0;
    };
}