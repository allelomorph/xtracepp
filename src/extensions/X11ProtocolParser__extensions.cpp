#include <string_view>
#include <unordered_map>

#include "X11ProtocolParser.hpp"

#include "protocol/extensions/big_requests.hpp"


namespace ext = protocol::extensions;

const std::unordered_map< std::string_view, X11ProtocolParser::_ExtensionTraits >
X11ProtocolParser::_extensions {
    { ext::big_requests::name,
      _ExtensionTraits(
          {
              { ext::big_requests::requests::opcodes::BIGREQENABLE,
                _RequestOpcodeTraits(
                    ext::big_requests::requests::names.at(
                        ext::big_requests::requests::opcodes::BIGREQENABLE ),
                    &X11ProtocolParser::_parseRequest<
                    ext::big_requests::requests::BigReqEnable >,
                    &X11ProtocolParser::_parseReply<
                    ext::big_requests::requests::BigReqEnable::Reply > )
              }
          } )
    }
};
