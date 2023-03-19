
#include "resource.hpp"
#include "filesystem_rnode.hpp"
#include "const/error_page.hpp"
#include "const_content.hpp"

namespace myhttpd::http {

    class default_rnode : public rnode {

    public:
        virtual void async_request(std::shared_ptr<request> req, request_handler handler) {
            auto rsp = std::make_shared<response>();
            rsp->set_status(404);
        }

    public:
        default_rnode() = default;

        virtual ~default_rnode() = default;
    };

    std::string get_suffix(std::string url) {
        auto offset = url.find_last_of('.');
        if (offset == std::string::npos) {
            return "default";
        } else {
            return url.substr(offset);
        }
    }

    std::shared_ptr<content> resource::_get_error_page(unsigned status) {
        switch (status) {
        case 400:
        case 401:
        case 402:
        case 403:
        case 404:
        case 405:
        case 406:
        case 407:
        case 408:
        case 409:
        case 410:
        case 411:
        case 412:
        case 413:
        case 414:
        case 415:
        case 416:
        case 417:
        case 500:
        case 501:
        case 502:
        case 503:
        case 504:
        case 505:
        default: return this->_error_pages[404];
        }
    }

    void resource::async_request(std::shared_ptr<request> req, request_handler handler) {
        if (req->get_url() == "/") {
            req->set_url(this->_default);
        }
        auto url = req->get_url();
        std::string longest_match_rnode = "<default>";
        auto longest_match_size = 0;
        for (auto& rnode : this->_rnodes) {
            if (url.starts_with(rnode.first)) {
                if (rnode.first.size() > longest_match_size) {
                    longest_match_rnode = rnode.first;
                }
            }
        }
        auto rnode_it = this->_rnodes.find(longest_match_rnode);
        auto sub_url = url.substr(rnode_it->first.size(), url.size() - rnode_it->first.size());
        rnode_it->second->async_request(req,
            [req, handler, this](std::shared_ptr<response> rsp) {
                if (!rsp->has_content() && rsp->get_status() >= 400) {
                    rsp->set_content(this->_get_error_page(rsp->get_status()));
                    rsp->insert_attribute("content-type", this->_mimedb[".html"]);
                }
                if (!rsp->contains_attribute("content-type")) {
                    rsp->insert_attribute("content-type", this->_mimedb[get_suffix(req->get_url())]);
                }
                handler(rsp);
            }
        );
    }

    void resource::config(tinyxml2::XMLElement* config) {

        this->_default = config->Attribute("default");

        this->_rnodes.insert(
            std::pair<std::string, std::unique_ptr<rnode>>("<default>", std::make_unique<default_rnode>())
        );
        auto nodes = config->FirstChildElement("rnodes");
        auto node = nodes->FirstChildElement();
        while (node) {
            std::string type = node->Name();
            std::string vpath = node->Attribute("virtual_path");
            std::string ppath = node->Attribute("physical_path");
            if (type == "filesystem") {
                this->_rnodes.insert(
                    std::pair<std::string, std::unique_ptr<rnode>>(vpath, std::make_unique<filesystem_rnode>(ppath))
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

        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                404, std::make_shared<const_content>(page_404_html, page_404_html_size)
            )
        );
    }
}

