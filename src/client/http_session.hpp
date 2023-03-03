#ifndef __HTTP_H__
#define __HTTP_H__

#include <memory>
#include <list>
#include <array>
#include <boost/regex.hpp>

#include "session.hpp"
#include "network/connection.hpp"

namespace myhttpd {

    class http_session: public session {
    private:
        std::unique_ptr<connection> _conn;
        terminated_handler _terminated_handler;
        std::list<std::array<char, 4096>> _buffers;
    private:
        //void _header_read_handler(const boost::system::error_code &code);
        void receive_request();
        //void receive_header();
        //void receive_body();
    public:
        http_session(std::unique_ptr<connection> conn);
        virtual ~http_session();
        virtual void start(terminated_handler handler);
    };
}

#endif // __HTTP_H__