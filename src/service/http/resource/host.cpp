#include <filesystem>
#include <list>

#include "host.hpp"
#include "../const/error_page.hpp"
#include "const_content.hpp"

#if ENABLE_FILESYSTEM

#include "filesystem/filesystem_rnode.hpp"
#endif

#if ENABLE_WSGI

#include "wsgi/wsgi_rnode.hpp"
#endif

namespace myhttpd::service::http {

    

    std::unique_ptr<response> host::_make_error(unsigned code, std::unique_ptr<request> req) {

        auto rsp = std::make_unique<response>(std::move(req));
        rsp->set_status(code);
        auto& ep = this->_error_pages[code];
        rsp->set_content(ep);
        rsp->insert_attribute("content-length", std::to_string(ep->get_content_langth()));
        rsp->insert_attribute("content-type", "text/html");
        return rsp;
    }

    void host::async_request(std::unique_ptr<request> req, request_handler handler) {

        auto url = req->get_url();

        if (url == "/") {

            url = this->_default;

        } else {

            std::filesystem::path path(url);

            std::size_t path_deepth = 0;

            for (auto &name: path) {
                
                if (name.filename() == "..") {

                    path_deepth--;

                } else {

                    path_deepth++;
                }

                if (path_deepth < 0) {

                    handler(this->_make_error(404, std::move(req)));
                    return;
                }
            }
        }

        std::string longest_match_rnode = "<default>";
        auto longest_match_size = 0;
        auto match_flag = false;

        for (auto& rnode : this->_rnodes) {

            if (url.starts_with(rnode.first)) {

                match_flag = true;

                if (rnode.first.size() > longest_match_size) {

                    longest_match_rnode = rnode.first;
                    longest_match_size = rnode.first.size();
                }
            }
        }

        if (!match_flag) {

            handler(this->_make_error(404, std::move(req)));
            return;
        }

        auto rnode_it = this->_rnodes.find(longest_match_rnode);
        auto sub_url = url.substr(rnode_it->first.size(), url.size() - rnode_it->first.size());

        if(longest_match_rnode != "/") {

            req->set_url(sub_url);

        } else {

            req->set_url(url);
        }

        rnode_it->second->async_request(std::move(req), 

            [handler, this](std::unique_ptr<response> rsp) {

                rsp->insert_attribute("host", this->_name);
                auto status = rsp->get_status();

                if (status >= 400) {

                    auto& ep = this->_error_pages[status];
                    rsp->set_content(ep);
                    rsp->insert_attribute("content-type", "text/html");

                }

                if (rsp->has_content()) {

                    auto attr_cl = rsp->find_attribute("content-lenth");

                    if (attr_cl != rsp->end_attribute()) {

                        rsp->erase_attribute(attr_cl);
                    }

                    auto content = rsp->get_content();
                    rsp->insert_attribute("content-length", std::to_string(content->get_content_langth()));
                }

                handler(std::move(rsp));
            }
        );
    }

    void host::_rnodes_init(tinyxml2::XMLElement* config) {

        if (!config) {

            return;
        }

        auto node_config = config->FirstChildElement("rnode");

        while (node_config) {

            std::string type = node_config->Attribute("type");
            auto vpath = node_config->Attribute("virtual_path");

            if (type == "filesystem") {

#if ENABLE_FILESYSTEM
                
                auto* fs_config = node_config->FirstChildElement("filesystem");
                std::string ppath = fs_config->Attribute("physical_path");
                typedef access_control<filesystem_rnode, std::string> secure_filesystem_rnode;
                auto node = std::make_unique<secure_filesystem_rnode>(node_config, this->_auth, ppath);
                auto pair = std::pair<std::string, std::unique_ptr<rnode>>(vpath, (std::unique_ptr<rnode>&&)std::move(node));
                this->_rnodes.insert(std::move(pair));
#endif

            } else if (type == "wsgi") {

#if ENABLE_WSGI

                auto* wsgi_config = node_config->FirstChildElement("wsgi");
                std::string module_path = wsgi_config->Attribute("module_path");
                typedef access_control<wsgi_rnode, std::string, std::string> secure_wsgi_rnode;
                auto node = std::make_unique<secure_wsgi_rnode>(node_config, this->_auth, module_path, vpath);
                auto pair = std::pair<std::string, std::unique_ptr<rnode>>(vpath, (std::unique_ptr<rnode>&&)std::move(node));
                this->_rnodes.insert(std::move(pair));
#endif 

            } else {

            }

            node_config = node_config->NextSiblingElement("rnode");
        }
    }

    void host::_error_pages_init(tinyxml2::XMLElement* config) {

    }

    host::host(
        tinyxml2::XMLElement* config,
        authentication& auth,
        std::array<std::shared_ptr<content>, 506> error_pages) :
        _error_pages(error_pages), _auth(auth) {

        this->_default = config->Attribute("default");
        this->_name = config->Attribute("name");
        this->_rnodes_init(config);
    }
}