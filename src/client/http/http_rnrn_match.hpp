#include <cctype>

namespace myhttpd {

    class http_rnrn_match {

    private:
        int status = 0;

    public:
        bool match(const char value);

        void reset();

    };
}