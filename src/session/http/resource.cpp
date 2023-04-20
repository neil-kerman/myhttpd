#include <glog/logging.h>

#include "resource.hpp"
#include "filesystem_rnode.hpp"
#include "wsgi_rnode.hpp"
#include "const/error_page.hpp"
#include "const_content.hpp"

namespace myhttpd::session::http {

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
                    longest_match_size = rnode.first.size();
                }
            }
        }

        auto rnode_it = this->_rnodes.find(longest_match_rnode);
        auto sub_url = url.substr(rnode_it->first.size(), url.size() - rnode_it->first.size());

        if (longest_match_rnode != "/") {

            req->set_url(sub_url);
        }

        rnode_it->second->async_request(req,
            [req, handler, this](std::shared_ptr<response> rsp) {

                if (!rsp->has_content() && rsp->get_status() >= 400) {

                    rsp->set_content(this->_get_error_page(rsp->get_status()));
                    rsp->insert_attribute("content-length", std::to_string(rsp->get_content()->get_size()));
                    rsp->insert_attribute("content-type", this->_mimedb[".html"]);
                }

                handler(rsp);
            }
        );
    }

    void resource::config(tinyxml2::XMLElement* config) {

        this->_error_pages_init();
        this->_mimedb_init();
        this->_rnodes_init(config->FirstChildElement("rnodes"));
        this->_default = config->Attribute("default");
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

    void resource::_rnodes_init(tinyxml2::XMLElement* config) {

        this->_rnodes.insert(
            std::pair<std::string, std::unique_ptr<rnode>>("<default>", std::make_unique<default_rnode>())
        );

        if (!config) {

            LOG(INFO) << "No rnode has set";
            return;
        }

        auto node = config->FirstChildElement();

        while (node) {

            std::string type = node->Name();

            if (type == "filesystem") {
                std::string vpath = node->Attribute("virtual_path");
                std::string ppath = node->Attribute("physical_path");
                this->_rnodes.insert(
                    std::pair<std::string, std::unique_ptr<rnode>>(vpath, std::make_unique<filesystem_rnode>(ppath))
                );
                LOG(INFO) << "Filesystem resource node added, virtual path: \"" + vpath + "\", physical path: \"" + ppath + "\"";

            } else if (type == "wsgi") {

                std::string vpath = node->Attribute("virtual_path");
                std::string module_path = node->Attribute("module_path");
                this->_rnodes.insert(
                    std::pair<std::string, std::unique_ptr<rnode>>(vpath, std::make_unique<wsgi_rnode>(module_path, vpath))
                );

            } else {

                LOG(ERROR) << "Unknown rnode type: " << type;
            }

            node = node->NextSiblingElement();
        }
    }

    void resource::_error_pages_init() {

        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                400, std::make_shared<const_content>(page_400_html, page_400_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                401, std::make_shared<const_content>(page_401_html, page_401_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                402, std::make_shared<const_content>(page_402_html, page_402_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                403, std::make_shared<const_content>(page_403_html, page_403_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                404, std::make_shared<const_content>(page_404_html, page_404_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                405, std::make_shared<const_content>(page_405_html, page_405_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                406, std::make_shared<const_content>(page_406_html, page_406_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                407, std::make_shared<const_content>(page_407_html, page_407_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                408, std::make_shared<const_content>(page_408_html, page_408_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                409, std::make_shared<const_content>(page_409_html, page_409_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                410, std::make_shared<const_content>(page_410_html, page_410_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                411, std::make_shared<const_content>(page_411_html, page_411_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                412, std::make_shared<const_content>(page_412_html, page_412_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                413, std::make_shared<const_content>(page_413_html, page_413_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                414, std::make_shared<const_content>(page_414_html, page_414_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                415, std::make_shared<const_content>(page_415_html, page_415_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                416, std::make_shared<const_content>(page_416_html, page_416_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                417, std::make_shared<const_content>(page_417_html, page_417_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                500, std::make_shared<const_content>(page_500_html, page_500_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                501, std::make_shared<const_content>(page_501_html, page_501_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                502, std::make_shared<const_content>(page_502_html, page_502_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                503, std::make_shared<const_content>(page_503_html, page_503_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                504, std::make_shared<const_content>(page_504_html, page_504_html_size)
            )
        );
        this->_error_pages.insert(
            std::pair<unsigned, std::shared_ptr<content>>(
                505, std::make_shared<const_content>(page_505_html, page_505_html_size)
            )
        );
    }

    std::shared_ptr<content> resource::_get_error_page(unsigned status) {

        switch (status) {

        case 400: return this->_error_pages[400];
        case 401: return this->_error_pages[401];
        case 402: return this->_error_pages[402];
        case 403: return this->_error_pages[403];
        case 404: return this->_error_pages[404];
        case 405: return this->_error_pages[405];
        case 406: return this->_error_pages[406];
        case 407: return this->_error_pages[407];
        case 408: return this->_error_pages[408];
        case 409: return this->_error_pages[409];
        case 410: return this->_error_pages[410];
        case 411: return this->_error_pages[411];
        case 412: return this->_error_pages[412];
        case 413: return this->_error_pages[413];
        case 414: return this->_error_pages[414];
        case 415: return this->_error_pages[415];
        case 416: return this->_error_pages[416];
        case 417: return this->_error_pages[417];
        case 500: return this->_error_pages[500];
        case 501: return this->_error_pages[501];
        case 502: return this->_error_pages[502];
        case 503: return this->_error_pages[503];
        case 504: return this->_error_pages[504];
        case 505: return this->_error_pages[505];
        default: return this->_error_pages[500];
        }
    }
}

