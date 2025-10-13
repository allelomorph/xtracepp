#ifndef PROTOCOL_RESPONSE_HPP
#define PROTOCOL_RESPONSE_HPP


#include "protocol/common_types.hpp"


namespace protocol {

// TBD provides generic header for Error, Reply, Event
struct Response {
    struct [[gnu::packed]] Header {
        uint8_t prefix;
    private:
        uint8_t _unused;
    public:
        CARD16  sequence_num;
    };

    enum {
        ERROR_PREFIX,
        REPLY_PREFIX
    };

    virtual ~Response() = 0;
};

}  // namespace protocol


#endif  // PROTOCOL_RESPONSE_HPP
