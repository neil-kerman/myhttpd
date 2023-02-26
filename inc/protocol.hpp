#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

class protocol {
public:
    virtual ~protocol() {}
    virtual void set_protocol_upgrad_event_handler();
};

#endif // __PROTOCOL_H__