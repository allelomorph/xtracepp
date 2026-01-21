#include <unordered_map>
#include <cstdint>

#include "X11ProtocolParser.hpp"
#include "protocol/errors.hpp"


using namespace protocol::errors;

const std::unordered_map< uint8_t, X11ProtocolParser::_ErrorCodeTraits >
X11ProtocolParser::_core_errors {
    { codes::REQUEST,         //  1
      _ErrorCodeTraits( names.at( codes::REQUEST ),
                        &X11ProtocolParser::_parseError< Request > ) },
    { codes::VALUE,           //  2
      _ErrorCodeTraits( names.at( codes::VALUE ),
                        &X11ProtocolParser::_parseError< Value > ) },
    { codes::WINDOW,          //  3
      _ErrorCodeTraits( names.at( codes::WINDOW ),
                        &X11ProtocolParser::_parseError< Window > ) },
    { codes::PIXMAP,          //  4
      _ErrorCodeTraits( names.at( codes::PIXMAP ),
                        &X11ProtocolParser::_parseError< Pixmap > ) },
    { codes::ATOM,            //  5
      _ErrorCodeTraits( names.at( codes::ATOM ),
                        &X11ProtocolParser::_parseError< Atom > ) },
    { codes::CURSOR,          //  6
      _ErrorCodeTraits( names.at( codes::CURSOR ),
                        &X11ProtocolParser::_parseError< Cursor > ) },
    { codes::FONT,            //  7
      _ErrorCodeTraits( names.at( codes::FONT ),
                        &X11ProtocolParser::_parseError< Font > ) },
    { codes::MATCH,           //  8
      _ErrorCodeTraits( names.at( codes::MATCH ),
                        &X11ProtocolParser::_parseError< Match > ) },
    { codes::DRAWABLE,        //  9
      _ErrorCodeTraits( names.at( codes::DRAWABLE ),
                        &X11ProtocolParser::_parseError< Drawable > ) },
    { codes::ACCESS,          // 10
      _ErrorCodeTraits( names.at( codes::ACCESS ),
                        &X11ProtocolParser::_parseError< Access > ) },
    { codes::ALLOC,           // 11
      _ErrorCodeTraits( names.at( codes::ALLOC ),
                        &X11ProtocolParser::_parseError< Alloc > ) },
    { codes::COLORMAP,        // 12
      _ErrorCodeTraits( names.at( codes::COLORMAP ),
                        &X11ProtocolParser::_parseError< Colormap > ) },
    { codes::GCONTEXT,        // 13
      _ErrorCodeTraits( names.at( codes::GCONTEXT ),
                        &X11ProtocolParser::_parseError< GContext > ) },
    { codes::IDCHOICE,        // 14
      _ErrorCodeTraits( names.at( codes::IDCHOICE ),
                        &X11ProtocolParser::_parseError< IdChoice > ) },
    { codes::NAME,            // 15
      _ErrorCodeTraits( names.at( codes::NAME ),
                        &X11ProtocolParser::_parseError< Name > ) },
    { codes::LENGTH,          // 16
      _ErrorCodeTraits( names.at( codes::LENGTH ),
                        &X11ProtocolParser::_parseError< Length > ) },
    { codes::IMPLEMENTATION,  // 17
      _ErrorCodeTraits( names.at( codes::IMPLEMENTATION ),
                        &X11ProtocolParser::_parseError< Implementation > ) }
};
