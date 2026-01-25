#include <unordered_map>
#include <string_view>

#include "X11ProtocolParser.hpp"
#include "Extensions.hpp"


const std::unordered_map< std::string_view, X11ProtocolParser::_ExtensionTraits >
X11ProtocolParser::_extensions {
};
