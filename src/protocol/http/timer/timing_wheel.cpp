

#include "timing_wheel.hpp"
#include "../utils.hpp"

namespace myhttpd::protocol::http {

    void timing_wheel::_set_timer() {

        this->_update_datetime();
        this->_raw_timer.expires_from_now(boost::posix_time::seconds(1));
        this->_raw_timer.async_wait(

            [this](const boost::system::error_code error) {

                auto* head = &(this->_timing_wheel.front());
                auto* node = head->next;

                while (node != head) {

                    auto* next = node->next;
                    node->handler();
                    delete node;
                    node = next;
                }

                this->_timing_wheel.pop_front();
                this->_timing_wheel.push_back(timeout_handler_list());
                head = &(this->_timing_wheel.back());
                head->previous = head;
                head->next = head;
                this->_set_timer();
            }
        );
    }

    void timing_wheel::_update_datetime() {

        boost::posix_time::ptime utc_datetime = boost::posix_time::second_clock::universal_time();
        this->_last_rfc1123_datetime = utils::ptime_to_http_date(utc_datetime);
    }

    void timing_wheel::cancel(token tk) {

        tk->previous->next = tk->next;
        tk->next->previous = tk->previous;
        delete tk;
    }

    timing_wheel::token timing_wheel::set(timeout_handler handler) {

        auto& head = this->_timing_wheel.back();
        auto* new_handler = new timeout_handler_list{ &head, head.next, handler };
        head.next = new_handler;
        new_handler->next->previous = new_handler;
        return new_handler;
    }

    std::string timing_wheel::get_current_rfc1123_datetime() {

        return this->_last_rfc1123_datetime;
    }

    timing_wheel::timing_wheel(boost::asio::io_context& ctx) :
        _ctx(ctx), _raw_timer(ctx) {

        for (int i = 0; i < 120; i++) {

            this->_timing_wheel.push_back(timeout_handler_list());
            auto& head = this->_timing_wheel.back();
            head.previous = &head;
            head.next = &head;
        }

        this->_set_timer();
    }
}