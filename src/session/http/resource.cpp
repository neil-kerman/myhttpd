
#include "resource.hpp"
#include "filesystem_rnode.hpp"
#include "const/error_page.hpp"
#include "const_content.hpp"

namespace myhttpd::http {

    std::string get_suffix(std::string url) {
        auto offset = url.find_last_of('.');
        if (offset == std::string::npos) {
            return "default";
        } else {
            return url.substr(offset);
        }
    }

    void resource::async_request(std::shared_ptr<request> req, request_handler handler) {
        if (req->get_url() == "/") {
            req->set_url(this->_default);
        }
        auto url = req->get_url();
        std::string longest_match_rnode;
        auto longest_match_size = longest_match_rnode.size();
        for (auto& rnode : this->_rnodes) {
            if (url.starts_with(rnode.first)) {
                if (rnode.first.size() > longest_match_size) {
                    longest_match_rnode = rnode.first;
                }
            }
        }
        if (longest_match_rnode.empty()) {
            auto rsp = std::make_shared<response>();
            rsp->set_status(404);
            rsp->set_content(std::make_shared<const_content>(page_404_html, page_404_html_size));
            handler(rsp);
        } else {
            auto rnode_it = this->_rnodes.find(longest_match_rnode);
            auto sub_url = url.substr(rnode_it->first.size(), url.size() - rnode_it->first.size());
            rnode_it->second->async_request(req, 
                [handler](std::shared_ptr<response> rsp) {
                    if (!rsp->has_content() && rsp->get_status() >= 400) { 
                        rsp->set_content(std::make_shared<const_content>(page_404_html, page_404_html_size));
                    }
                    handler(rsp);
                }
            );
        }
    }

    void resource::config(tinyxml2::XMLElement* config) {
        this->_default = config->Attribute("default");
        auto nodes = config->FirstChildElement("rnodes");
        auto node = nodes->FirstChildElement();
        while (node) {
            std::string type = node->Name();
            std::string vpath = node->Attribute("virtual_path");
            std::string ppath = node->Attribute("physical_path");
            if (type == "filesystem") {
                if (vpath == "/") {
                    ppath += "/";
                }
                this->_rnodes.insert(
                    std::pair<std::string, std::unique_ptr<rnode>>(
                        vpath,
                        std::make_unique<filesystem_rnode>(ppath)
                    )
                );
            } else {

            }
            node = node->NextSiblingElement();
        }

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
}

