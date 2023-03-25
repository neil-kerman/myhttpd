#include <Python.h>
#include <boost/python/exec.hpp>
#include <boost/python/dict.hpp>
#include <filesystem>
#include <boost/bind.hpp>
#include <glog/logging.h>
#include <memory>
#include <algorithm>
#include <boost/asio.hpp>

#include "wsgi_rnode.hpp"
#include "transmitting_content.hpp"

namespace myhttpd::http {

    void wsgi_rnode::async_request(std::shared_ptr<request> req, request_handler handler) {
        using namespace boost::python;
        auto header = dict();
        header["REQUEST_METHOD"] = object(request::method_to_string(req->get_method()));
        header["SCRIPT_NAME"] = object(this->_virtual_path);
        header["PATH_INFO"] = object(req->get_url());
        header["SERVER_NAME"] = "localhost";
        header["SERVER_PORT"] = "80";
        header["QUERY_STRING"] = object(req->get_query_string());
        header["CONTENT_TYPE"] = "";
        header["SERVER_PROTOCOL"] = "http";
        header["wsgi.version"] = exec_statement("(1, 0)");
        header["wsgi.input"] = exec_statement("bytearray()");
        header["wsgi.errors"];
        header["wsgi.multithread"] = exec_statement("False");
        header["wsgi.run_once"] = exec_statement("False");

        try {
            auto rsp = std::make_shared<http::response>();
            auto rsp_content = (*(this->_application))(
                header, make_function(
                    boost::function<object(str, list)>(
                        [&](str status, list header) {
                            rsp->set_title(req->get_version() + " " + extract<std::string>(status)());
                            auto len = extract<int>(header.attr("__len__")())();
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
                auto vec = std::make_shared<std::vector<char>>();
                vec->resize(size);
                std::memcpy(vec->data(), buf, size);
                auto tran_contnet = std::make_shared<transmitting_content>(vec);
                auto err = boost::system::error_code();
                tran_contnet->deliver(err);
                rsp->set_content(tran_contnet);
                LOG(INFO) << size;
            }
            handler(rsp);
        }
        catch (boost::python::error_already_set const&) {
            PyErr_Print();
        }
    }

    wsgi_rnode::wsgi_rnode(std::string module_path, std::string virtual_path)
    : _virtual_path(virtual_path) {
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