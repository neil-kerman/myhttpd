

#define __MYHTTPD_UNIT_TEST__

#include "client/http/transceiver_1_1.hpp"
#include "network/connection.hpp"

const char req_text[] = 
"GET / HTTP/1.1\r\n\
Host: www.google.com\r\n\
User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko / 20100101 Firefox / 110.0\r\n\
Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, image / avif, image / webp, */*;q=0.8\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
DNT: 1\r\n\
Connection: keep-alive\r\n\
Sec-Fetch-Dest: document\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-User: ?1\r\n\
TE: trailers\r\n\
\r\nGET / HTTP/1.1\r\n\
Host: www.google.com\r\n\
User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko / 20100101 Firefox / 110.0\r\n\
Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, image / avif, image / webp, */*;q=0.8\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
DNT: 1\r\n\
Connection: keep-alive\r\n\
Sec-Fetch-Dest: document\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-User: ?1\r\n\
TE: trailers\r\n\
\r\nGET / HTTP/1.1\r\n\
Host: www.google.com\r\n\
User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko / 20100101 Firefox / 110.0\r\n\
Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, image / avif, image / webp, */*;q=0.8\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
DNT: 1\r\n\
Connection: keep-alive\r\n\
Sec-Fetch-Dest: document\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-User: ?1\r\n\
TE: trailers\r\n\
\r\nGET / HTTP/1.1\r\n\
Host: www.google.com\r\n\
User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko / 20100101 Firefox / 110.0\r\n\
Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, image / avif, image / webp, */*;q=0.8\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
DNT: 1\r\n\
Connection: keep-alive\r\n\
Sec-Fetch-Dest: document\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-User: ?1\r\n\
TE: trailers\r\n\
\r\nGET / HTTP/1.1\r\n\
Host: www.google.com\r\n\
User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko / 20100101 Firefox / 110.0\r\n\
Accept: text / html, application / xhtml + xml, application / xml; q = 0.9, image / avif, image / webp, */*;q=0.8\r\n\
Accept-Language: en-US,en;q=0.5\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
DNT: 1\r\n\
Connection: keep-alive\r\n\
Sec-Fetch-Dest: document\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-User: ?1\r\n\
TE: trailers\r\n\
\r\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

namespace myhttpd {

    class fake_connection : public connection {
    public:
        fake_connection() = default;

        virtual ~fake_connection() {}

        virtual void async_read_some(char* buf, std::size_t size, read_handler handler) {
            using namespace myhttpd;
            static std::size_t ptr = 0;
            std::memcpy(buf, req_text + ptr, 50);
            ptr += 50;
            handler(connection::success, 50);
        }

        virtual void async_write_some(const char* buf, std::size_t size, write_handler handler) {

        }

        virtual void async_wait(wait_type type, wait_handler handler) {
        
        }
    };

}

#include <gtest/gtest.h>

TEST(_header_receive_handler, receive) {
    using namespace myhttpd;
    std::unique_ptr<connection> conn = std::make_unique<fake_connection>();
    auto tran = std::make_unique<transceiver_1_1>(conn);
    //transceiver_1_1 tran((std::unique_ptr<connection>&)conn);
    tran->async_receive([](transceiver::error_code code, std::unique_ptr<message> msg) {
        EXPECT_EQ(msg->header.size(), 14);
    });
}