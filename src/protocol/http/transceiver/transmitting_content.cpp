#include "transmitting_content.hpp"

namespace myhttpd::protocol::http {


    network::connection::mutable_buffer transmitting_content::get_buffer(std::size_t size) {

        if (size == 0) {

            return { 

                this->_buffer.data() + this->_available_bytes, 
                this->get_content_langth() - this->_available_bytes 
            };

        } else {

            return {

                this->_buffer.data() + this->_available_bytes, size
            };
        }
    }

    void transmitting_content::cancel() {

        this->_requested = false;
        this->_read_handler(content_error_code::error, { nullptr, 0 });
    }
    
    std::size_t transmitting_content::commit(std::size_t size) {

        this->_available_bytes += size;

        if (this->_requested) {

            if (this->_requested_offset < this->_available_bytes
                && this->_requested_size <= this->_available_bytes - this->_requested_offset) {

                this->_requested = false;
                this->_read_handler(
                    content_error_code::success, 
                    { this->_buffer.data() + this->_requested_offset, this->_requested_size }
                );
            }
        }

        return this->get_content_langth() - this->_available_bytes;
    }

    std::size_t transmitting_content::get_content_langth() {

        return this->_buffer.size();
    }

    void transmitting_content::async_read(read_handler handler, std::size_t offset, std::size_t size) {

        this->_requested = true;

        if (!size) {

            this->_requested_size = this->get_content_langth() - offset;
        }

        this->_requested_offset = offset;
        this->_read_handler = handler;
        this->commit(0);
    }

    transmitting_content::transmitting_content(std::size_t size): _buffer(size) {

    }
}