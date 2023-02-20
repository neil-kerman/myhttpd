#ifndef __CONNECTION_H__
#define __CONNECTION_H__

class connection {
public:
    virtual ~connection() {}
    virtual void async_disconnect() = 0;
    virtual void async_write() = 0;
    virtual void async_read() = 0;
    virtual void async_wait() = 0;
    virtual std::size_t available() = 0;
};

#endif // __CONNECTION_H__