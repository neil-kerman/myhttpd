#include <boost/filesystem.hpp>

#include "filesystem_content.hpp"
#include "filesystem_rnode.hpp"

namespace myhttpd::session::http {

	bool filesystem_rnode::_exists(std::string url) {

		std::string abs_path = this->_path + url;
		return boost::filesystem::exists(abs_path) && boost::filesystem::is_regular_file(abs_path);
	}

	void http::filesystem_rnode::_do_option(std::shared_ptr<request> req, request_handler handler) {

	}

	void http::filesystem_rnode::_do_get(std::shared_ptr<request> req, request_handler handler) {

		auto rsp = std::make_shared<response>();

		if (this->_exists(req->get_url())) {

			auto content = std::make_shared<filesystem_content>(this->_path + req->get_url());
			rsp->set_content(content);
			rsp->insert_attribute("accept-ranges", "bytes");

			if (req->contains_attribute("range")) {

				auto value = req->find_attribute("range")->second;
				auto offset = value.find('=') + 1;
				auto end_offset = value.find('-', offset);
				auto size = end_offset - offset;
				auto begin_bytes_str = value.substr(offset, size);
				auto begin_bytes = std::stoll(begin_bytes_str);
				content->set_range_begin(begin_bytes);


				if (end_offset == value.size() - 1) {

					offset = end_offset + 1;
					end_offset = value.find('\r', offset);
					size = end_offset - offset;
					auto end_bytes_str = value.substr(offset, size);
					auto end_bytes = std::stoll(end_bytes_str);
					content->set_range_end(end_bytes);
				}

				rsp->set_status(206);
				rsp->insert_attribute("content-range", value + "/" + std::to_string(content->get_size()));

			} else {

				rsp->set_status(200);
			}

		} else {

			rsp->set_status(404);
		}

		handler(rsp);
	}

	void http::filesystem_rnode::_do_head(std::shared_ptr<request> req, request_handler handler) {

		this->_do_get(req, 

			[handler](std::shared_ptr<response> rsp) {
				
				rsp->set_content(nullptr);
				handler(rsp);
			}
		);
	}

	void http::filesystem_rnode::_do_post(std::shared_ptr<request> req, request_handler handler) {

		this->_do_get(req, handler);
	}

	void http::filesystem_rnode::_do_put(std::shared_ptr<request> req, request_handler handler) {

	}

	void http::filesystem_rnode::_do_delete(std::shared_ptr<request> req, request_handler handler) {

	}

	void http::filesystem_rnode::_do_trace(std::shared_ptr<request> req, request_handler handler) {

		auto rsp = std::make_shared<response>();
		rsp->set_content(req->get_content());

		if (rsp->contains_attribute("content-length")) {

			rsp->insert_attribute("content-length", req->find_attribute("content-length")->second);
		}

		handler(rsp);
	}

	void filesystem_rnode::async_request(std::shared_ptr<request> req, request_handler handler) {

		switch (req->get_method()) {

		case request::method::OPTIONS: this->_do_option(req, handler); break;
		case request::method::GET: this->_do_get(req, handler); break;
		case request::method::HEAD: this->_do_head(req, handler); break;
		case request::method::POST: this->_do_post(req, handler); break;
		case request::method::PUT: this->_do_put(req, handler); break;
		case request::method::DELETE: this->_do_delete(req, handler); break;
		case request::method::TRACE: this->_do_trace(req, handler); break;

		default:

			break;
		}
	}

	filesystem_rnode::filesystem_rnode(std::string path): 
		_path(path) {

	}
}