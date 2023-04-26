#ifndef WSGI_RNODE_HPP
#define WSGI_RNODE_HPP

#include <boost/function.hpp>
#include <boost/function_types/components.hpp>

namespace boost {
    namespace python {
        namespace detail {
            // get_signature overloads must be declared before including
            // boost/python.hpp.  The declaration must be visible at the
            // point of definition of various Boost.Python templates during
            // the first phase of two phase lookup.  Boost.Python invokes the
            // get_signature function via qualified-id, thus ADL is disabled.

            /// @brief Get the signature of a boost::function.
            template <typename Signature>
            inline typename boost::function_types::components<Signature>::type
                get_signature(boost::function<Signature>&, void* = 0) {
                return typename boost::function_types::components<Signature>::type();
            }

        } // namespace detail
    } // namespace python
} // namespace boost

#include <string>
#include <boost/asio.hpp>
#include <boost/python.hpp>

#include "../../resource/rnode.hpp"

namespace myhttpd::session::http {

    class wsgi_rnode : public rnode {

    private:
        std::string _virtual_path;

        std::shared_ptr<boost::python::api::object_attribute> _application = nullptr;



    private:
        void _call_application(std::shared_ptr<request> req, request_handler handler);

    public:
        virtual void async_request(std::unique_ptr<request> req, request_handler handler);

    public:
        wsgi_rnode(std::string module_path, std::string virtual_path);

        virtual ~wsgi_rnode();
    };
}

#endif // WSGI_RNODE_HPP
