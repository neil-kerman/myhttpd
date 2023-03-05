#ifndef __HTTP_H__
#define __HTTP_H__

#include <memory>
#include <list>
#include <array>
#include <boost/regex.hpp>

#include "http_transceiver_1_1.hpp"
#include "client/session.hpp"
#include "network/connection.hpp"
#include "client/http/resource.hpp"

namespace myhttpd {

    class http_session: public session {

    private:
        std::unique_ptr<connection> _conn;

        std::unique_ptr<http_transceiver> _transceiver;

        terminated_handler _terminated_handler;

        resource &_resource;
        
    private:
        void receive_request();
        
    public:
        http_session(std::unique_ptr<connection> conn, resource &res);

        virtual ~http_session();

        virtual void start(terminated_handler handler);

    };
}

#endif // __HTTP_H__