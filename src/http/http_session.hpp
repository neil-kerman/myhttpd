#ifndef __HTTP_H__
#define __HTTP_H__

#include <memory>
#include <vector>
#include <list>

#include "../session.hpp"
#include "../connection/connection.hpp"

namespace myhttpd {

    class http_session: public session {
    private:
        std::unique_ptr<connection> _conn;
        myhttpd::session::terminated_handler _terminated_handler;
    private:
        //void _header_read_handler(const boost::system::error_code &code);
        void _read_header();
        void _header_buffer_expand();
    public:
        http_session(std::unique_ptr<connection> conn);
        virtual ~http_session();
        virtual void start(myhttpd::session::terminated_handler handler);
    };    
}

#endif // __HTTP_H__