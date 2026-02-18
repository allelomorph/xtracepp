#include <unordered_map>

#include <cstdint>

#include "X11ProtocolParser.hpp"

#include "protocol/errors.hpp"


namespace er = protocol::errors;

const std::unordered_map< uint8_t, X11ProtocolParser::_ErrorCodeTraits >
X11ProtocolParser::_core_errors {
    { er::codes::REQUEST,         //  1
      _ErrorCodeTraits( er::names.at( er::codes::REQUEST ),
                        &X11ProtocolParser::_parseError< er::Request > ) },
    { er::codes::VALUE,           //  2
      _ErrorCodeTraits( er::names.at( er::codes::VALUE ),
                        &X11ProtocolParser::_parseError< er::Value > ) },
    { er::codes::WINDOW,          //  3
      _ErrorCodeTraits( er::names.at( er::codes::WINDOW ),
                        &X11ProtocolParser::_parseError< er::Window > ) },
    { er::codes::PIXMAP,          //  4
      _ErrorCodeTraits( er::names.at( er::codes::PIXMAP ),
                        &X11ProtocolParser::_parseError< er::Pixmap > ) },
    { er::codes::ATOM,            //  5
      _ErrorCodeTraits( er::names.at( er::codes::ATOM ),
                        &X11ProtocolParser::_parseError< er::Atom > ) },
    { er::codes::CURSOR,          //  6
      _ErrorCodeTraits( er::names.at( er::codes::CURSOR ),
                        &X11ProtocolParser::_parseError< er::Cursor > ) },
    { er::codes::FONT,            //  7
      _ErrorCodeTraits( er::names.at( er::codes::FONT ),
                        &X11ProtocolParser::_parseError< er::Font > ) },
    { er::codes::MATCH,           //  8
      _ErrorCodeTraits( er::names.at( er::codes::MATCH ),
                        &X11ProtocolParser::_parseError< er::Match > ) },
    { er::codes::DRAWABLE,        //  9
      _ErrorCodeTraits( er::names.at( er::codes::DRAWABLE ),
                        &X11ProtocolParser::_parseError< er::Drawable > ) },
    { er::codes::ACCESS,          // 10
      _ErrorCodeTraits( er::names.at( er::codes::ACCESS ),
                        &X11ProtocolParser::_parseError< er::Access > ) },
    { er::codes::ALLOC,           // 11
      _ErrorCodeTraits( er::names.at( er::codes::ALLOC ),
                        &X11ProtocolParser::_parseError< er::Alloc > ) },
    { er::codes::COLORMAP,        // 12
      _ErrorCodeTraits( er::names.at( er::codes::COLORMAP ),
                        &X11ProtocolParser::_parseError< er::Colormap > ) },
    { er::codes::GCONTEXT,        // 13
      _ErrorCodeTraits( er::names.at( er::codes::GCONTEXT ),
                        &X11ProtocolParser::_parseError< er::GContext > ) },
    { er::codes::IDCHOICE,        // 14
      _ErrorCodeTraits( er::names.at( er::codes::IDCHOICE ),
                        &X11ProtocolParser::_parseError< er::IDChoice > ) },
    { er::codes::NAME,            // 15
      _ErrorCodeTraits( er::names.at( er::codes::NAME ),
                        &X11ProtocolParser::_parseError< er::Name > ) },
    { er::codes::LENGTH,          // 16
      _ErrorCodeTraits( er::names.at( er::codes::LENGTH ),
                        &X11ProtocolParser::_parseError< er::Length > ) },
    { er::codes::IMPLEMENTATION,  // 17
      _ErrorCodeTraits( er::names.at( er::codes::IMPLEMENTATION ),
                        &X11ProtocolParser::_parseError< er::Implementation > ) }
};
