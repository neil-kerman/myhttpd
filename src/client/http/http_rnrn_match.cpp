#include "http_rnrn_match.hpp"

namespace myhttpd {

    bool http_rnrn_match::match(const char value) {
        switch (this->status) {
        case 0:
            if (value == '\r') {
                this->status++;
            }
            break;
        case 1:
            if (value == '\n') {
                this->status++;
            } else if (value == '\r') {
                this->status = 1;
            } else {
                this->status = 0;
            }
            break;
        case 2:
            if (value == '\r') {
                this->status++;
            } else {
                this->status = 0;
            }
            break;
        case 3:
            if (value == '\n') {
                this->status++;
            } else if (value == '\r') {
                this->status = 1;
            } else {
                this->status = 0;
            }
            break;
        default:
            break;
            }
            return this->status == 4;
    }

    void http_rnrn_match::reset() {
        this->status = 0;
    }
}
