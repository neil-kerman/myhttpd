#ifndef __RNODE_H__
#define __RNODE_H__

#include <string>
#include <functional>
#include <memory>
#include <boost/asio/buffer.hpp>

#include "content.hpp"

namespace asio = boost::asio;
/* Resource responding event handler */
typedef std::function<
    void (asio::const_buffer header, std::unique_ptr<content> con)> resp_handler;

class rnode {
private:
    std::string _vpath;
public:
    rnode(std::string vpath);
    virtual ~rnode();
    std::string get_vpath();
    virtual void async_request(resp_handler handler) = 0;
};

#endif // __RNODE_H__