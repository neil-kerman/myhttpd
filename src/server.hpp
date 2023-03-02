#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <map>
#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <tinyxml2.h>

#include "acceptor.hpp"
#include "session.hpp"
#include "http/resource.hpp"
#include "connection/connection.hpp"

namespace myhttpd {
    class server {
    private:
        boost::asio::io_context _io;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;
        std::list<std::unique_ptr<acceptor>> _acceptors;
        std::map<boost::uuids::uuid, std::unique_ptr<session>> _sessions;
    private:
    public:
        server(tinyxml2::XMLElement *config);
        void start();
        void event_loop();
    };
}

#endif // __SERVER_H__