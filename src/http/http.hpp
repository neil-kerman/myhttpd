#ifndef __HTTP_H__
#define __HTTP_H__

#include "../session.hpp"
#include "../connection/connection.hpp"

namespace myhttpd::http {
    using myhttpd::connection::connection;
    using namespace myhttpd;
    class http: public session {
    public:
        http(std::unique_ptr<connection> conn, st_handler st);
        virtual ~http();
        virtual void start();
    };    
}



#endif // __HTTP_H__