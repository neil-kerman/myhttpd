#ifndef __HTTP_H__
#define __HTTP_H__

#include "session.hpp"

class http: public session {
public:
    http(std::unique_ptr<connection> conn, st_handler st);
    virtual start();
    virtual ~http();
};

#endif // __HTTP_H__