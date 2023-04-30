#include <glog/logging.h>

#include "resource.hpp"
#include "const_content.hpp"
#include "../const/error_page.hpp"
#include "host.hpp"

namespace myhttpd::session::http {

    class default_rnode : public rnode {

    public:
        virtual void async_request(std::unique_ptr<request> req, request_handler handler) {

            auto rsp = std::make_unique<response>(std::move(req));
            rsp->set_status(404);
            handler(std::move(rsp));
        }

    public:
        default_rnode() = default;

        virtual ~default_rnode() = default;
    };

    std::unique_ptr<response> resource::_make_error(unsigned code, std::unique_ptr<request> req) {

        auto rsp = std::make_unique<response>(std::move(req));
        rsp->set_status(code);
        auto& ep = this->_error_pages[code];
        rsp->set_content(ep);
        rsp->insert_attribute("content-length", std::to_string(ep->get_size()));
        rsp->insert_attribute("content-type", this->_mimedb[".html"]);
        return rsp;
    }

    void resource::async_request(std::unique_ptr<request> req, request_handler handler) {

        if (req->contains_attribute("host")) {

            auto host_it = this->_hosts.find(req->find_attribute("host")->second);

            if (host_it != this->_hosts.end()) {

                host_it->second.async_request(std::move(req), handler);

            } else {

                handler(this->_make_error(404, std::move(req)));
            }

        } else {

            handler(this->_make_error(404, std::move(req)));
        }
    }

    void resource::config(tinyxml2::XMLElement* config) {

        this->_error_pages_init(config);
        this->_mimedb_init();
        this->_hosts_init(config);
    }

    void resource::_mimedb_init() {

        tinyxml2::XMLDocument mimedb_doc;
        mimedb_doc.LoadFile("../config/myhttpd.mimedb.xml");
        auto mime_types = mimedb_doc.RootElement();
        auto mime_type = mime_types->FirstChildElement();

        while (mime_type) {

            auto suffix = mime_type->Attribute("suffix");
            auto type = mime_type->Attribute("type");
            this->_mimedb.insert(std::pair<std::string, std::string>(suffix, type));
            mime_type = mime_type->NextSiblingElement();
        }
    }

    void resource::_hosts_init(tinyxml2::XMLElement* config) {

        config = config->FirstChildElement("hosts");
        auto node = config->FirstChildElement("host");
        
        while (node) {

            std::string name = node->Attribute("name");
            auto host_node = std::pair<std::string, host>(name, host(this->_error_pages, this->_mimedb));
            host_node.second.config(node);
            this->_hosts.insert(std::move(host_node));
            node = node->NextSiblingElement();
        }
    }

    void resource::_error_pages_init(tinyxml2::XMLElement* config) {

        this->_error_pages[400] = std::make_shared<const_content>(page_400_html, page_400_html_size);
        this->_error_pages[401] = std::make_shared<const_content>(page_401_html, page_401_html_size);
        this->_error_pages[402] = std::make_shared<const_content>(page_402_html, page_402_html_size);
        this->_error_pages[403] = std::make_shared<const_content>(page_403_html, page_403_html_size);
        this->_error_pages[404] = std::make_shared<const_content>(page_404_html, page_404_html_size);
        this->_error_pages[405] = std::make_shared<const_content>(page_405_html, page_405_html_size);
        this->_error_pages[406] = std::make_shared<const_content>(page_406_html, page_406_html_size);
        this->_error_pages[407] = std::make_shared<const_content>(page_407_html, page_407_html_size);
        this->_error_pages[408] = std::make_shared<const_content>(page_408_html, page_408_html_size);
        this->_error_pages[409] = std::make_shared<const_content>(page_409_html, page_409_html_size);
        this->_error_pages[410] = std::make_shared<const_content>(page_410_html, page_410_html_size);
        this->_error_pages[411] = std::make_shared<const_content>(page_411_html, page_411_html_size);
        this->_error_pages[412] = std::make_shared<const_content>(page_412_html, page_412_html_size);
        this->_error_pages[413] = std::make_shared<const_content>(page_413_html, page_413_html_size);
        this->_error_pages[414] = std::make_shared<const_content>(page_414_html, page_414_html_size);
        this->_error_pages[415] = std::make_shared<const_content>(page_415_html, page_415_html_size);
        this->_error_pages[416] = std::make_shared<const_content>(page_416_html, page_416_html_size);
        this->_error_pages[417] = std::make_shared<const_content>(page_417_html, page_417_html_size);
        this->_error_pages[500] = std::make_shared<const_content>(page_500_html, page_500_html_size);
        this->_error_pages[501] = std::make_shared<const_content>(page_501_html, page_501_html_size);
        this->_error_pages[502] = std::make_shared<const_content>(page_502_html, page_502_html_size);
        this->_error_pages[503] = std::make_shared<const_content>(page_503_html, page_503_html_size);
        this->_error_pages[504] = std::make_shared<const_content>(page_504_html, page_504_html_size);
        this->_error_pages[505] = std::make_shared<const_content>(page_505_html, page_505_html_size);
    }
}

