#include <Python.h>
#include <boost/python/exec.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <filesystem>
#include <boost/bind.hpp>
#include <memory>
#include <algorithm>
#include <boost/asio.hpp>

#include "wsgi_rnode.hpp"
#include "../../transceiver/transmitting_content.hpp"

namespace myhttpd::service::http {

    void wsgi_rnode::_call_application(
        std::unique_ptr<request> req, network::connection::const_buffer buf, request_handler handler) {

        using namespace boost::python;
        auto header = dict();
        header["REQUEST_METHOD"] = object(request::method_to_string(req->get_method()));
        header["SCRIPT_NAME"] = object(this->_virtual_path);
        header["PATH_INFO"] = object(req->get_url());
        header["SERVER_NAME"] = object(req->find_attribute("host")->second);
        header["SERVER_PORT"] = object("80");
        header["QUERY_STRING"] = object(req->get_query_string());
        header["GATEWAY_INTERFACE"] = "WSGI/1.0";
        header["SERVER_PROTOCOL"] = object(req->get_version());

        if (req->contains_attribute("content-language")) {

            header["CONTENT_LENGTH"] = object(req->find_attribute("content-language")->second);
        }

        if (req->contains_attribute("accept")) {

            header["HTTP_ACCEPT"] = object(req->find_attribute("accept")->second);
        }

        if (req->contains_attribute("accept-charset")) {

            header["HTTP_ACCEPT_CHARSET"] = object(req->find_attribute("accept-charset")->second);
        }

        if (req->contains_attribute("accept-encoding")) {

            header["HTTP_ACCEPT_ENCODING"] = object(req->find_attribute("accept-encoding")->second);
        }

        if (req->contains_attribute("accept-language")) {

            header["HTTP_ACCEPT_LANGUAGE"] = object(req->find_attribute("accept-language")->second);
        }

        if (req->contains_attribute("forward")) {

            header["HTTP_FORWARDED"] = object(req->find_attribute("forward")->second);
        }

        if (req->contains_attribute("user-agent")) {

            header["HTTP_USER_AGENT"] = object(req->find_attribute("user-agent")->second);
        }

        if (req->contains_attribute("referer")) {

            header["HTTP_REFERER"] = object(req->find_attribute("referer")->second);
        }

        if (req->contains_attribute("origin")) {

            header["HTTP_ORIGIN"] = object(req->find_attribute("origin")->second);
        }

        if (req->contains_attribute("cookie")) {

            header["HTTP_COOKIE"] = object(req->find_attribute("cookie")->second);
        }

        if (req->has_content()) {

            if (req->contains_attribute("content-type")) {

                header["CONTENT_TYPE"] = object(req->find_attribute("content-type")->second);

            } else {

                header["CONTENT_TYPE"] = "text/plain";
            }
        }
        list version;
        version.insert(0, 0);
        version.insert(0, 1);
        header["wsgi.version"] = import("builtins").attr("tuple")(version);

        if (req->has_content()) {

            auto content = req->get_content();
            auto bytearray = import("builtins").attr("bytearray")(str((const char*)buf.data, buf.size));
            header["wsgi.input"] = bytearray;
            import("builtins").attr("print")(bytearray);

        } else {

            header["wsgi.input"] = import("builtins").attr("bytearray")();
        }

        header["wsgi.errors"];
        header["wsgi.multithread"] = object(true);
        header["wsgi.run_once"] = object(false);

        try {

            auto rsp = std::make_unique<http::response>(std::move(req));
            auto rsp_content = (*(this->_application))(
                header, make_function(
                    boost::function<object(str, list)>(
                        [&rsp](str status, list header) {

                            rsp->set_status(std::stoi(extract<std::string>(status)()));
                            auto len = extract<int>(import("builtins").attr("len")(header))();

                            for (int i = 0; i < len; i++) {

                                auto attribute = header[i];
                                auto attr_name = extract<std::string>(attribute[0]);
                                auto attr_value = extract<std::string>(attribute[1]);
                                rsp->insert_attribute(attr_name, attr_value);
                            }

                            return long_(0);
                        }
                    )
                )
            );

            if (rsp->contains_attribute("content-length")) {

                auto bytes = ((object)rsp_content.attr("content"));
                std::size_t size = PyBytes_Size(bytes.ptr());
                auto buf = PyBytes_AsString(bytes.ptr());
                auto tran_contnet = std::make_shared<transmitting_content>(size);
                auto tran_buf = tran_contnet->get_buffer(size);
                std::memcpy(tran_buf.data, buf, size);
                tran_contnet->commit(size);
                rsp->set_content(tran_contnet);
            }
           
            handler(std::move(rsp));

        } catch (boost::python::error_already_set const&) {

            PyErr_Print();
        }
    }

    void wsgi_rnode::async_request(std::unique_ptr<request> req, request_handler handler) {

        if (req->has_content()) {

            auto req_sptr = std::make_shared<std::unique_ptr<request>>(std::move(req));
            auto content = (*req_sptr)->get_content();
            content->async_read(
                
                [this, req_sptr, handler](const content::content_error_code error, network::connection::const_buffer buf) {
                
                    this->_call_application(std::move(*req_sptr), buf, handler);
                }
            );

        } else {

            this->_call_application(std::move(req), {0, 0}, handler);
        }
    }

    wsgi_rnode::wsgi_rnode(std::string module_path, std::string virtual_path): 
        _virtual_path(virtual_path) {

        Py_Initialize();
        auto path = std::filesystem::absolute(std::filesystem::path(module_path));
        auto module_name = path.filename();
        path = path.parent_path();

        try {

            auto sys = boost::python::import("sys");
            sys.attr("path").attr("append")(boost::python::str(path.string()));
            this->_application = std::make_shared<boost::python::api::object_attribute>(
                boost::python::import(boost::python::str(module_name.string() + ".wsgi")).attr("application")
            );

        } catch (boost::python::error_already_set const&) {

            PyErr_Print();
        }
    }

    wsgi_rnode::~wsgi_rnode() {

        //Py_Finalize();
    }
}