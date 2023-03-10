

#include "resource.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd::http {

    void resource::async_request(std::unique_ptr<message> req, request_handler handler) {
        std::string url = req->get_url();
        for (auto& node : this->_rnodes) {
            const std::string &node_path = node.first;
            if (url.starts_with(node_path)) {
                node.second->async_request(url.substr(node_path.size(), url.size() - node_path.size()), std::move(req), handler);
                break;
            }
        }
        std::unique_ptr<message> rsp = std::make_unique<message>();


    }

    void resource::config(tinyxml2::XMLElement* config) {
        auto nodes = config->FirstChildElement("rnodes");
        auto node = nodes->FirstChildElement();
        while (node) {
            std::string type = node->Name();
            std::string vpath = node->Attribute("virtual_path");
            std::string ppath = node->Attribute("physical_path");
            if (type == "filesystem") {
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
        nodes = config->FirstChildElement("default");
        node = nodes->FirstChildElement();
        while (node) {
            std::string url = node->GetText();
            this->_default.push_back(std::move(url));
            node = node->NextSiblingElement();
        }
    }
}

