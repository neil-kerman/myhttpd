#include <string>

#include "acceptor_factory.hpp"
#include "tcp_acceptor.hpp"
#include "tls_acceptor.hpp"

namespace myhttpd::network {

    std::unique_ptr<acceptor> acceptor_facory::create_acceptor(tinyxml2::XMLElement* config, boost::asio::io_context &ctx, server& ser) {
        std::string name = config->Name();
        std::string address = config->Attribute("address");
        int port = config->IntAttribute("port");
        std::unique_ptr<acceptor> ac;
        if (name == "tcp") {
            ac.reset(new tcp_acceptor(address, port, ctx, ser));
            return ac;
        } else if (name == "tls") {
            std::string tls_version = config->Attribute("tls_version");
            std::string key_file = config->Attribute("key_file");
            std::string cert_file = config->Attribute("cert_file");
            using boost::asio::ssl::context;
            std::unique_ptr<context> tls_ctx;
            if (tls_version == "tls1.0") {
                tls_ctx.reset(new context(context::tlsv1_server));
            } else if (tls_version == "tls1.1") {
                tls_ctx.reset(new context(context::tlsv11_server));
            } else if (tls_version == "tls1.2") {
                tls_ctx.reset(new context(context::tlsv12_server));
            } else if (tls_version == "tls1.3") {
                tls_ctx.reset(new context(context::tlsv13_server));
            } else {
                return nullptr;
            }
            tls_ctx->use_private_key_file(key_file, context::pem);
            tls_ctx->use_certificate_file(cert_file, context::pem);
            ac.reset(new tls_acceptor(address, port, ctx, std::move(*tls_ctx), ser));
            return ac;
        } else {
            return nullptr;
        }
    }
}