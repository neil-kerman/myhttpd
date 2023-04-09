#ifndef SESSION_SERVER
#define SESSION_SERVER

namespace myhttpd::session {

    class server {

    public:
        virtual void request_termination() = 0;

    public:
        virtual ~server() = default;
    };
}

#endif // SESSION_SERVER