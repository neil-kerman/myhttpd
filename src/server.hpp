#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <map>
#include <memory>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <tinyxml2.h>

#include "network/acceptor.hpp"
#include "client/session.hpp"
#include "client/http/resource.hpp"

namespace myhttpd {
    class server {

    private:
        boost::asio::io_context _io;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _work_guard;

        std::list<std::unique_ptr<acceptor>> _acceptors;

        std::map<boost::uuids::uuid, std::unique_ptr<session>> _sessions;

        http::resource _resource;

    private:

    public:
        server(tinyxml2::XMLElement *config);

        void start();

        void event_loop();
    };
}



#endif // __SERVER_H__