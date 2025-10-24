#include <cassert>
#include <ctime>  // localtime
#include <cstdint>

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "X11ProtocolParser.hpp"
#include "Connection.hpp"
#include "protocol/common_types.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/atoms.hpp"
#include "protocol/requests.hpp"
#include "protocol/connection_setup.hpp"


std::string
X11ProtocolParser::_formatCommonType( const protocol::DRAWABLE drawable,
                                      const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    // defaulting to treating like WINDOW
    assert( ( drawable.window.data & protocol::WINDOW::ZERO_BITS ) == 0 );
    return _formatInteger( drawable.window.data, name_range );
}

std::string
X11ProtocolParser::_formatCommonType( const protocol::FONTABLE fontable,
                                      const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    // defaulting to treating like FONT
    assert( ( fontable.font.data & protocol::FONT::ZERO_BITS ) == 0 );
    return _formatInteger( fontable.font.data, name_range );
}

std::string
X11ProtocolParser::_formatCommonType( const protocol::ATOM atom,
                                      const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    assert( ( atom.data & protocol::ATOM::ZERO_BITS ) == 0 );
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::zero_none ||
                name_range.names == protocol::requests::GetProperty::type_names );
    }
    if ( name_range.in( atom.data ) ) {
        return _formatInteger( atom.data, name_range );
    }
    const auto it { _interned_atoms.find( atom.data ) };
    return fmt::format( "{}({})", _formatInteger( atom.data ),
                        it == _interned_atoms.end() ? "unrecognized atom" :
                        fmt::format( "{:?}", it->second ) );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::TIMESTAMP time,
                                      const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::time );
    }
    const std::string hex_str {
        _formatInteger( time.data, {}, _ValueTraits::BITMASK ) };  // force hex
    if ( name_range.in( time.data ) ) {
        return fmt::format( "{}({})", hex_str, name_range.at( time.data ) );
    }
    if ( settings.relativetimestamps ) {
        static constexpr uint32_t TIMESTAMP_TICKS_PER_SEC { 1000 };
        const std::time_t time_ (
            ( ( time.data - settings.ref_TIMESTAMP ) / TIMESTAMP_TICKS_PER_SEC )
            + settings.ref_unix_time );
        // RFC 3339 UTC format "yyyy-mm-ddThh:mm:ssZ":
        // https://www.rfc-editor.org/rfc/rfc3339#section-5.6
        return fmt::format( "{}({:%FT:%TZ}UTC)", hex_str,
                            *std::localtime( &time_ ) );
    }
    return hex_str;
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::BITGRAVITY bitgravity,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatInteger( bitgravity.data,
                           { protocol::BITGRAVITY::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::WINGRAVITY wingravity,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatInteger( wingravity.data,
                           { protocol::WINGRAVITY::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::BOOL bool_,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    return _formatInteger( bool_.data,
                           { protocol::BOOL::enum_names } );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::SETofEVENT setofevent,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofevent.data & protocol::SETofEVENT::ZERO_BITS ) == 0 );
    return _formatInteger( setofevent.data,
                           { protocol::SETofEVENT::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::SETofPOINTEREVENT setofpointerevent,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofpointerevent.data &
              protocol::SETofPOINTEREVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatInteger( setofpointerevent.data,
                           { protocol::SETofPOINTEREVENT::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::SETofDEVICEEVENT setofdeviceevent,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofdeviceevent.data &
              protocol::SETofDEVICEEVENT::ZERO_BITS ) == 0 );
    // no need to denote a max flag index for the enum if zero bits validated
    return _formatInteger( setofdeviceevent.data,
                           { protocol::SETofDEVICEEVENT::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::KEYSYM keysym,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    // TBD encoding is convoluted, see:
    //   https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#keysym_encoding
    return _formatInteger( keysym.data,
                           {}, _ValueTraits::BITMASK );  // force hex
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::KEYCODE keycode,
                                      const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::key );
    }
    return _formatInteger( keycode.data, name_range );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::BUTTON button,
                                      const X11ProtocolParser::_EnumNameRange name_range/* = {}*/ ) {
    if ( !name_range.empty() ) {
        assert( name_range.names == protocol::enum_names::button );
    }
    return _formatInteger( button.data, name_range );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::SETofKEYBUTMASK setofkeybutmask,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    assert( ( setofkeybutmask.data & protocol::SETofKEYBUTMASK::ZERO_BITS ) == 0 );
    return _formatInteger( setofkeybutmask.data,
                           { protocol::SETofKEYBUTMASK::flag_names },
                           _ValueTraits::BITMASK );
}

template<>
std::string
X11ProtocolParser::_formatCommonType( const protocol::SETofKEYMASK setofkeymask,
                                      const X11ProtocolParser::_EnumNameRange/* name_range = {}*/ ) {
    if ( setofkeymask.data == protocol::SETofKEYMASK::ANYMODIFIER ) {
        if ( settings.verbose ) {
            return fmt::format(
                "{}({})", _formatInteger( setofkeymask.data,
                                          {}, _ValueTraits::BITMASK ),
                protocol::SETofKEYMASK::anymodifier_flag_name );
        }
        return std::string( protocol::SETofKEYMASK::anymodifier_flag_name );
    }
    assert( ( setofkeymask.data & protocol::SETofKEYMASK::ZERO_BITS ) == 0 );
    return _formatInteger( setofkeymask.data,
                           { protocol::SETofKEYMASK::flag_names },
                           _ValueTraits::BITMASK );
}
