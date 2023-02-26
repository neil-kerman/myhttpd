#ifndef __HTTP_H__
#define __HTTP_H__

#include "protocol.hpp"

class http: public protocol {
public:
    http();
    virtual ~http();
};

#endif // __HTTP_H__