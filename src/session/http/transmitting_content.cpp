#include "transmitting_content.hpp"

namespace myhttpd::session::http {

    void transmitting_content::deliver(const asio_error_code &error) {
        this->_is_ready = true;
        this->_error = error;
        while (!this->_wait_list.empty()) {
            auto& handler = this->_wait_list.front();
            handler(error, {this->_data->data(), this->_data->size()});
            this->_wait_list.pop_front();
        }
    }

    std::size_t transmitting_content::get_size() {
        return this->_data->size();
    }

    const void* transmitting_content::get_data() {
        return this->_data->data();
    }

    void transmitting_content::async_wait_ready(wait_handler handler) {
        if (this->_is_ready) {
            handler(this->_error, {this->_data->data(), this->_data->size()});
        } else {
            this->_wait_list.push_back(handler);
        }
    }

    transmitting_content::transmitting_content(std::shared_ptr<std::vector<char>> data)
    : _data(data) {}
}