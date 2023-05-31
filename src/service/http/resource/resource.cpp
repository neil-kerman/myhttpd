#include "resource.hpp"
#include "resource.hpp"

#include "resource.hpp"
#include "const_content.hpp"
#include "../const/error_page.hpp"
#include "host.hpp"
#include "../security/access_control.hpp"

namespace myhttpd::service::http {

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

    std::unique_ptr<response> resource::_make_error(unsigned status, std::unique_ptr<request> req) {

        auto rsp = std::make_unique<response>(std::move(req));
        rsp->set_status(status);
        auto& ep = this->_error_pages[status];
        rsp->set_content(ep);
        rsp->insert_attribute("content-length", std::to_string(ep->get_content_langth()));
        rsp->insert_attribute("content-type", "text/html");
        return rsp;
    }

    void resource::async_request(std::unique_ptr<request> req, request_handler handler) {

        if (req->contains_attribute("host")) {

            auto host_it = this->_hosts.find(req->find_attribute("host")->second);

            if (host_it != this->_hosts.end()) {

                host_it->second->async_request(std::move(req), 
                    
                    [this, handler](std::unique_ptr<response> rsp) {
                        
                        auto status = rsp->get_status();

                        if (status >= 400 && !rsp->has_content()) {

                            auto& ep = this->_error_pages[status];
                            rsp->set_content(ep);
                            rsp->insert_attribute("content-length", std::to_string(ep->get_content_langth()));
                            rsp->insert_attribute("content-type", "text/html");
                        }

                        handler(std::move(rsp));
                    }
                );

            } else {

                handler(this->_make_error(404, std::move(req)));
            }

        } else {

            handler(this->_make_error(404, std::move(req)));
        }
    }

    resource::resource(tinyxml2::XMLElement* config) {

        this->_error_pages_init(config);
        this->_auth_init();
        this->_hosts_init(config);
    }

    void resource::_hosts_init(tinyxml2::XMLElement* config) {

        auto node = config->FirstChildElement("host");
        
        while (node) {

            std::string name = node->Attribute("name");
            typedef access_control<host, tinyxml2::XMLElement*, authentication&, 
                std::array<std::shared_ptr<content>, 506>> secure_host;
            auto host_node = std::make_unique<secure_host>(node, *(this->_auth), node, *(this->_auth), this->_error_pages);
            auto pair = std::pair<std::string, std::unique_ptr<host>>(name, (std::unique_ptr<host>&&)std::move(host_node));
            this->_hosts.insert(std::move(pair));
            node = node->NextSiblingElement("host");
        }
    }

    void myhttpd::service::http::resource::_auth_init() {

        tinyxml2::XMLDocument users_config;
        users_config.LoadFile("../config/myhttpd.users.xml");
        auto config = users_config.RootElement();
        this->_auth.reset(new authentication(config));
    }

    void resource::_error_pages_init(tinyxml2::XMLElement* config) {

        this->_error_pages[400] = std::make_shared<const_content>(page_400_html, (std::size_t)page_400_html_size);
        this->_error_pages[401] = std::make_shared<const_content>(page_401_html, (std::size_t)page_401_html_size);
        this->_error_pages[402] = std::make_shared<const_content>(page_402_html, (std::size_t)page_402_html_size);
        this->_error_pages[403] = std::make_shared<const_content>(page_403_html, (std::size_t)page_403_html_size);
        this->_error_pages[404] = std::make_shared<const_content>(page_404_html, (std::size_t)page_404_html_size);
        this->_error_pages[405] = std::make_shared<const_content>(page_405_html, (std::size_t)page_405_html_size);
        this->_error_pages[406] = std::make_shared<const_content>(page_406_html, (std::size_t)page_406_html_size);
        this->_error_pages[407] = std::make_shared<const_content>(page_407_html, (std::size_t)page_407_html_size);
        this->_error_pages[408] = std::make_shared<const_content>(page_408_html, (std::size_t)page_408_html_size);
        this->_error_pages[409] = std::make_shared<const_content>(page_409_html, (std::size_t)page_409_html_size);
        this->_error_pages[410] = std::make_shared<const_content>(page_410_html, (std::size_t)page_410_html_size);
        this->_error_pages[411] = std::make_shared<const_content>(page_411_html, (std::size_t)page_411_html_size);
        this->_error_pages[412] = std::make_shared<const_content>(page_412_html, (std::size_t)page_412_html_size);
        this->_error_pages[413] = std::make_shared<const_content>(page_413_html, (std::size_t)page_413_html_size);
        this->_error_pages[414] = std::make_shared<const_content>(page_414_html, (std::size_t)page_414_html_size);
        this->_error_pages[415] = std::make_shared<const_content>(page_415_html, (std::size_t)page_415_html_size);
        this->_error_pages[416] = std::make_shared<const_content>(page_416_html, (std::size_t)page_416_html_size);
        this->_error_pages[417] = std::make_shared<const_content>(page_417_html, (std::size_t)page_417_html_size);
        this->_error_pages[500] = std::make_shared<const_content>(page_500_html, (std::size_t)page_500_html_size);
        this->_error_pages[501] = std::make_shared<const_content>(page_501_html, (std::size_t)page_501_html_size);
        this->_error_pages[502] = std::make_shared<const_content>(page_502_html, (std::size_t)page_502_html_size);
        this->_error_pages[503] = std::make_shared<const_content>(page_503_html, (std::size_t)page_503_html_size);
        this->_error_pages[504] = std::make_shared<const_content>(page_504_html, (std::size_t)page_504_html_size);
        this->_error_pages[505] = std::make_shared<const_content>(page_505_html, (std::size_t)page_505_html_size);
    }
}

