#include <functional>
#include <memory>

#include "http_request.hpp"
#include "http_response.hpp"

namespace myhttpd {
    
    class http_transceiver {
    public:
        enum error_code {
            success = 0,
            error = -1,
        };
        typedef std::function<void (error_code code, std::unique_ptr<http_request> req)> receive_handler;
        typedef std::function<void (error_code code)> send_handler;
    public:
        virtual ~http_transceiver() {}
        virtual void async_receive(receive_handler handler) = 0;
        virtual void async_send(std::unique_ptr<http_response> response, send_handler handler) = 0;
    };
}