

#include "resource.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd {

    void resource::async_request(std::unique_ptr<http_request> req, request_handler handler) {
        
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
    }
}

