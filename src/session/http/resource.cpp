

#include "resource.hpp"
#include "filesystem_rnode.hpp"
#include "const/error_page.hpp"

namespace myhttpd::http {

    std::string get_url(message& msg) {
        auto& title = msg.get_title();
        auto offset = title.find(' ') + 1;
        auto size = title.find(' ', offset) - offset;
        return title.substr(offset, size);
    }

    std::string get_suffix(std::string url) {
        auto offset = url.find_last_of('.');
        if (offset == std::string::npos) {
            return "default";
        } else {
            return url.substr(offset);
        }
    }

    void resource::async_request(std::unique_ptr<message> req, request_handler handler) {
        std::string url = get_url(*req);
        if (url == "/" || url == "") {
            url = this->_default;
        }
        for (auto& rnode : this->_rnodes) {
            if (url.starts_with(rnode.first)) {
                auto sub_url = url.substr(rnode.first.size(), url.size() - rnode.first.size());
                rnode.second->async_request(sub_url, std::move(req), handler);
                return;
            }
        }
        auto rsp = std::make_shared<message>();
        rsp->set_title("HTTP/1.1 404 Not Found");
        handler(rsp);
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

