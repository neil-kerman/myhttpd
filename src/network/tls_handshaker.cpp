
#include "tls_handshaker.hpp"
#include "tls_connection.hpp"

using namespace boost::asio::ip;
namespace ssl = boost::asio::ssl;

namespace myhttpd::network {

    std::unique_ptr<tls_handshaker> tls_handshaker::create_instance(tinyxml2::XMLElement* config) {

        std::string tag = listener::get_listener_tag_from_config(config);
        std::string tls_version = config->Attribute("tls_version");
        std::string key_file = config->Attribute("key_file");
        std::string cert_file = config->Attribute("cert_file");
        using boost::asio::ssl::context;
        std::unique_ptr<context> tls_ctx = nullptr;

        if (tls_version == "tls1.0") {

            tls_ctx.reset(new context(context::tlsv1_server));

        } else if (tls_version == "tls1.1") {

            tls_ctx.reset(new context(context::tlsv11_server));

        } else if (tls_version == "tls1.2") {

            tls_ctx.reset(new context(context::tlsv12_server));

        } else if (tls_version == "tls1.3") {

            tls_ctx.reset(new context(context::tlsv13_server));

        } else {

            return std::make_unique<tls_handshaker>(tag, std::move(*tls_ctx));
        }

        tls_ctx->use_private_key_file(key_file, context::pem);
        tls_ctx->use_certificate_file(cert_file, context::pem);
        return std::make_unique<tls_handshaker>(tag, std::move(*tls_ctx));
    }

    void tls_handshaker::async_handshake(boost::asio::ip::tcp::socket soc, handshaking_handler handler) {

        typedef boost::asio::ssl::stream<tcp::socket> tls_stream;
        auto stream = std::make_shared<tls_stream>(std::move(soc), this->_tls_ctx);
        stream->async_handshake(tls_stream::server,

            [stream, handler](const asio_error_code error) {

                if (!error) {

                    handler(std::make_unique<tls_connection>(std::move(*stream)));

                } else {

                }
            }
        );
    }

    tls_handshaker::tls_handshaker(listener_tag tag, boost::asio::ssl::context tls_ctx)
        : handshaker(tag), _tls_ctx(std::move(tls_ctx)){

    }
}