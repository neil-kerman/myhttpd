#include "transmitting_content.hpp"

namespace myhttpd::http {

    void transmitting_content::deliver(error_code error) {
        this->_is_ready = true;
        while (!this->_wait_list.empty()) {
            auto& handler = this->_wait_list.front();
            handler(content::error_code::successful, this->_data->data(), this->_data->size());
            this->_wait_list.pop_front();
        }
    }

    void transmitting_content::async_wait_ready(wait_handler handler) {
        if (this->_is_ready) {
            handler(this->_error, this->_data->data(), this->_data->size());
        } else {
            this->_wait_list.push_back(handler);
        }
    }

    transmitting_content::transmitting_content(std::shared_ptr<std::vector<char>> data)
    : _data(data) {}

    transmitting_content::~transmitting_content() {}
}