#ifndef X11PROTOCOLPARSER_HPP
#define X11PROTOCOLPARSER_HPP


#include <tuple>  // tuple_sz
#include <string>
#include <string_view>
#include <type_traits>  // enable_if_t remove_reference_t
#include <vector>
#include <algorithm>  // max
#include <limits>
#include <optional>
#include <unordered_map>

#include <cassert>

#include <stdio.h>  // stdout

#include <fmt/format.h>

#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/common_types.hpp"
#include "protocol/requests.hpp"
#include "protocol/predefined_atoms.hpp"


class X11ProtocolParser {
private:
    // TBD change to PAD_ALIGN_SZ to differentiate from VALUE default size
    static constexpr size_t _ALIGN { 4 };

    // TBD settings imported from server class
    FILE* _log_fs            { stdout };
    bool  _multiline         {};
    bool  _verbose           {};
    bool  _readwritedebug    {};
    bool  _denyallextensions {};

    // TBD formatting
    char             _separator { ' ' };  // '\n'  for multiline
    std::string_view _equals    { "=" };  // " = " for multiline

    struct _StashedAtomID {
        uint32_t conn_id {};
        uint16_t seq_num {};

        bool operator==( const _StashedAtomID& other ) const {
            return this->conn_id == other.conn_id &&
                this->seq_num == other.seq_num;
        }
    };
    struct _StashedAtomIDHash {
        size_t operator()(const _StashedAtomID& sa_id) const noexcept {
            const size_t h1 { std::hash<uint32_t>{}( sa_id.conn_id ) };
            const size_t h2 { std::hash<uint16_t>{}( sa_id.seq_num ) };
            return h1 ^ (h2 << 1); // or use boost::hash_combine
        }
    };
    // TBD index atoms by conn id and seq num between InternAtom request and reply
    std::unordered_map<_StashedAtomID, std::string_view, _StashedAtomIDHash>
    _stashed_atoms;
    // TBD once InternAtom replies with server ATOM, properly map to ATOM
    std::unordered_map<uint32_t, std::string_view> _interned_atoms;

    // only atom string known at InternAtom request parsing
    void
    _stashAtom( const _StashedAtomID sa_id, const std::string_view atom_str );
    // when parsing InternAtom reply, then string and ATOM can be joined
    void
    _internStashedAtom( const _StashedAtomID sa_id, const protocol::ATOM atom );
    std::optional<std::string_view>
    _getInternedAtom(const protocol::ATOM);

    std::string
    _bufferHexDump( const uint8_t* data, const size_t sz );

    static constexpr uint32_t _TAB_SZ { 2 };  // in spaces
    std::string_view
    _tabIndent( const uint32_t tab_ct );

    // "where E is some expression, and pad(E) is the number of bytes needed to
    //   round E up to a multiple of four."
    inline size_t _padToAlignment( const size_t n, const size_t align ) {
        return n + ( ( align - ( n % align ) ) % align );
    };
    inline size_t _pad( const size_t n ) {
        return _padToAlignment( n, _ALIGN );
    }

    // template< typename T >
    // auto _parseSimpleType( const uint8_t** data, size_t* bytes_parsed ) ->
    //     std::enable_if_t< std::is_arithmetic_v< T >, T > {
    //     T val { *reinterpret_cast< T* >( *data ) };
    //     *bytes_parsed += sizeof( T );
    //     *data += sizeof( T );
    //     return val;
    // }

    // TBD for CURSOR COLORMAP ATOM VISUALID WINDOW PIXMAP DRAWABLE FONT GCONTEXT FONTABLE
    static constexpr uint32_t
    _TOP_3_OF_32_BITS { 0xE0000000 };

    template < typename T >
    inline auto _top_three_bits_zero( const T value ) ->
        std::enable_if_t< std::is_integral_v< T > && sizeof( T ) == 4,
            bool > {
        return ( ( value & _TOP_3_OF_32_BITS ) == 0 );
    }

    // TBD sentinel necessary?
    static constexpr uint32_t _UNINITIALIZED {
        std::numeric_limits< uint32_t >::max() };

    // TBD maybe for readability pass in _EnumRange{ min, max } instead
    // TBD [u]intXX_t CARDXX INTXX
    template < typename ValueT >
    auto _formatInteger(
        const ValueT value,
        const std::vector< std::string_view >& enum_names = {},
        uint32_t max_enum = _UNINITIALIZED,
        uint32_t min_enum = _UNINITIALIZED ) ->
        std::enable_if_t< std::is_integral_v< ValueT >, std::string > {

        std::string name_str {};
        // TBD how much do we need min and max params?
        if ( !enum_names.empty() ) {
            assert( value != _UNINITIALIZED );
            if ( max_enum == _UNINITIALIZED )
                max_enum = enum_names.size() - 1;
            if ( min_enum == _UNINITIALIZED )
                min_enum = 0;
            if ( value <= ValueT( max_enum ) && value >= ValueT( min_enum ) )
                name_str = std::string( enum_names[ value ] );
        }
        if ( _verbose ) {
            // fmt counts "0x" as part of width when using '#'
            static constexpr size_t hex_width { ( sizeof( value ) * 2 ) + 2 };
            const std::string hex_str {
                fmt::format( "{:#0{}x}", std::make_unsigned_t< ValueT >( value ), hex_width ) };
            return name_str.empty() ? hex_str :
                fmt::format( "{}({})", hex_str, name_str );
        }
        return name_str.empty() ? fmt::format( "{:d}", value ) : name_str;
    }

    template < typename MaskT >
    auto _formatBitmask(
        const MaskT mask,
        const std::vector< std::string_view >& flag_names = {},
        uint32_t max_flag_i = _UNINITIALIZED ) ->
        std::enable_if_t< std::is_integral_v< MaskT >, std::string > {
        assert( !std::is_signed_v< MaskT > );
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( mask ) * 2 ) + 2 };
        std::string hex_str { fmt::format( "{:#0{}x}", mask, hex_width ) };
        std::string flag_str;
        if ( !flag_names.empty() ) {
            if ( max_flag_i == _UNINITIALIZED )
                max_flag_i = flag_names.size() - 1;
            for ( uint32_t i {}; i <= max_flag_i; ++i ) {
                if ( mask & ( 1 << i ) ) {
                    flag_str.append( flag_str.empty() ? "" : "," );
                    flag_str.append( flag_names[i] );
                }
            }
        }
        if ( _verbose ) {
            return flag_str.empty() ? hex_str :
                fmt::format( "{}({})", hex_str, flag_str );
        }
        return flag_str.empty() ? hex_str : flag_str;
    }

    std::string
    _formatCommonType( const protocol::TIMESTAMP time,
                       const std::vector< std::string_view >& enum_names = {} );

    std::string
    _formatCommonType( const protocol::CURSOR cursor,
                       const std::vector< std::string_view >& enum_names = {} );

    // COLORMAP could use zero_none or zero_copy_from_parent
    std::string
    _formatCommonType( const protocol::COLORMAP colormap,
                       const std::vector< std::string_view >& enum_names = {} );

    // ATOM could use zero_none or property_atom
    std::string
    _formatCommonType( Connection* conn,
                       const protocol::ATOM atom,
                       const std::vector< std::string_view >& enum_names = {} );

    // VISUALID could use zero_none or zero_copy_from_parent
    std::string
    _formatCommonType( const protocol::VISUALID visualid,
                       const std::vector< std::string_view >& enum_names = {} );

    // WINDOW could use zero_none, event_destination, or input_focus
    std::string
    _formatCommonType( const protocol::WINDOW window,
                       const std::vector< std::string_view >& enum_names = {} );

    // PIXMAP could use zero_copy_from_parent, window_attribute_background_pixmap, or zero_none; just use _formatInteger
    std::string
    _formatCommonType( const protocol::PIXMAP pixmap,
                       const std::vector< std::string_view >& enum_names = {} );

    // TBD seemingly never used with enum names in protocol?
    inline std::string
    _formatCommonType( const protocol::DRAWABLE drawable,
                       const std::vector< std::string_view >& enum_names = {} ) {
        return _formatInteger( drawable.window.data, enum_names );
    }

    std::string
    _formatCommonType( const protocol::FONT font,
                       const std::vector< std::string_view >& enum_names = {} );

    // TBD inlining due to no examples found in standard of named enum
    inline std::string
    _formatCommonType( const protocol::GCONTEXT gcontext ) {
        return _formatInteger( gcontext.data );
    }

    // TBD FONTABLE?

    inline std::string
    _formatCommonType( const protocol::BITGRAVITY bitgravity ) {
        return _formatInteger( bitgravity.data,
                               protocol::enum_names::bitgravity );
    }

    inline std::string
    _formatCommonType( const protocol::WINGRAVITY wingravity ) {
        return _formatInteger( wingravity.data,
                               protocol::enum_names::wingravity );
    }

    inline std::string
    _formatCommonType( const protocol::BOOL bool_ ) {
        return _formatInteger( bool_.data,
                               protocol::enum_names::bool_ );
    }

    std::string
    _formatCommonType( const protocol::SETofEVENT setofevent );
    std::string
    _formatCommonType( const protocol::SETofPOINTEREVENT setofpointerevent );
    std::string
    _formatCommonType( const protocol::SETofDEVICEEVENT setofdeviceevent );

    std::string
    _formatCommonType( const protocol::KEYCODE keycode,
                       const std::vector< std::string_view >& enum_names = {} );

    std::string
    _formatCommonType( const protocol::BUTTON button,
                       const std::vector< std::string_view >& enum_names = {} );
    std::string
    _formatCommonType( const protocol::SETofKEYMASK setofkeymask );
    std::string
    _formatCommonType( const protocol::SETofKEYBUTMASK setofkeybutmask );

    std::string
    _formatCommonType( const protocol::POINT point );
    std::string
    _formatCommonType( const protocol::RECTANGLE rectangle );
    std::string
    _formatCommonType( const protocol::ARC arc );

    std::string
    _formatCommonType(
        const protocol::requests::QueryFont::CHARINFO charinfo );
    std::string
    _formatCommonType(
        Connection* conn,
        const protocol::requests::QueryFont::FONTPROP fontprop );

    struct _ParsingOutputs {
        std::string str       {};
        uint32_t bytes_parsed {};
    };

    // TBD can we generalize these into _parseLISTof<T>?
    _ParsingOutputs
    _parseLISTofSTR( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofCARD8( const uint8_t* data, const uint16_t n );
    inline _ParsingOutputs
    _parseLISTofBYTE( const uint8_t* data, const uint16_t n ) {
        return _parseLISTofCARD8( data, n );
    }
    _ParsingOutputs
    _parseLISTofCARD32( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofRECTANGLE( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofPOINT( const uint8_t* data, const uint16_t n );
    // TBD should this be encapsulated elsewhere? SEGMENT is not common type, but part of PolySegment request
    _ParsingOutputs
    _parseLISTofSEGMENT( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofARC( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofCOLORITEM( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofKEYSYM( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofATOM( Connection* conn, const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofKEYCODE( const uint8_t* data, const uint16_t n );

    _ParsingOutputs
    _parseLISTofTEXTITEM8( const uint8_t* data, const size_t sz );
    _ParsingOutputs
    _parseLISTofTEXTITEM16( const uint8_t* data, const size_t sz );

    _ParsingOutputs
    _parseLISTofWINDOW( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofTIMECOORD( const uint8_t* data, const uint16_t n );
    // TBD FONTPROP is a shared between two requests
    _ParsingOutputs
    _parseLISTofFONTPROP( Connection* conn, const uint8_t* data, const uint16_t n );
    // TBD CHARINFO is a shared between two requests
    _ParsingOutputs
    _parseLISTofCHARINFO( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofCOLORMAP( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofRGB( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofHOST( const uint8_t* data, const uint16_t n );

    struct _EnumTraits {
    private:
        inline static const std::vector< std::string_view > _EMPTY_NAMESET;
    public:
        const std::vector< std::string_view >& names { _EMPTY_NAMESET };
        const uint32_t min { _UNINITIALIZED };
        const uint32_t max { _UNINITIALIZED };
        const bool is_mask {};

        _EnumTraits() {}
        _EnumTraits(
            const std::vector<std::string_view>& names_,
            const bool is_mask_ = false,
            const uint32_t max_ = _UNINITIALIZED,
            const uint32_t min_ = _UNINITIALIZED ) :
            names( names_ ), is_mask( is_mask_ ), max( max_ ), min( min_ ) {}
    };

    template < typename TupleT >
    struct _LISTofVALUEParsingInputs {
        const TupleT& types;
        const std::vector< std::string_view >& names;
        // TBD how about tuple of format funcs instead?
        const std::vector< _EnumTraits >& traits;
        const uint32_t tab_ct;
        size_t name_width {};

        _LISTofVALUEParsingInputs() = delete;
        _LISTofVALUEParsingInputs(
            const TupleT& types_, const std::vector<std::string_view>& names_,
            const std::vector< _EnumTraits >& traits_,
            const uint32_t tab_ct_ ) :
            types( types_ ), names( names_ ),
            traits( traits_ ), tab_ct( tab_ct_ ) {
            assert( std::tuple_size< TupleT >{} == names.size() &&
                    names.size() == traits.size() );
            for ( const std::string_view& value_name : names ) {
                name_width = std::max( name_width, value_name.size() );
            }
        }
    };

    template < typename T >
    struct is_variable_enum_common_type : public std::false_type {};
    // TBD explicit template specializations need to be outside class declaration

    // TBD [u]intXX_t CARDXX INTXX
    template < typename ValueT >
    inline auto _formatVALUE(
        const ValueT value, const _EnumTraits& traits ) ->
        std::enable_if_t< std::is_integral_v< ValueT >, std::string > {
        return traits.is_mask ?
            _formatBitmask( value, traits.names, traits.max ) :
            _formatInteger( value, traits.names, traits.max );
    }

    // TBD two fmtCT args (uncertain enum_names)
    // PIXMAP WINDOW VISUALID COLORMAP
    template < typename ValueT >
    inline auto _formatVALUE(
        const ValueT value, const _EnumTraits& traits ) ->
        std::enable_if_t< is_variable_enum_common_type< ValueT >::value, std::string > {
        return _formatCommonType( value, traits.names );
    }

    // TBD one fmtCT arg
    // TIMESTAMP CURSOR ATOM FONT BITGRAVITY WINGRAVITY BOOL SETofEVENT SETofPOINTEREVENT SETofDEVICEEVENT KEYCODE BUTTON SETofKEYMASK SETofKEYBUTMASK POINT RECTANGLE ARC
    template < typename ValueT >
    inline auto _formatVALUE(
        const ValueT value, const _EnumTraits& /*traits*/ ) ->
        std::enable_if_t< !std::is_integral_v< ValueT > &&
                          !is_variable_enum_common_type< ValueT >::value, std::string > {
        return _formatCommonType( value );
    }

    // TBD not efficient to have static vars in templated function...
    static constexpr size_t _VALUE_ENCODING_SZ { sizeof( protocol::CARD32 ) };

    // tuple iteration allows for run time taversal of heterogeneous list of types
    // TBD all this may not be necessary - could we just parse all as uint32_t and cast as necessary?
    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args >
    auto _parseLISTofVALUE(
        const uint32_t value_mask,
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        const uint8_t* /*data*/, _ParsingOutputs* outputs ) ->
        std::enable_if_t< I == sizeof...( Args ), void > {
        if ( value_mask != 0 ) {
            outputs->str += _multiline ? _tabIndent( inputs.tab_ct - 1 ) : " ";
        }
        outputs->str += ']';
    }

    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args >
    auto _parseLISTofVALUE(
        const uint32_t value_mask,
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        const uint8_t* data, _ParsingOutputs* outputs ) ->
        std::enable_if_t< I < sizeof...( Args ), void > {
        if ( I == 0 ) {
            outputs->str += '[';
            if ( value_mask != 0 ) {
                outputs->str += _multiline ? '\n' : ' ';
            }
        }
        using ValueT = std::remove_reference_t<
            decltype( std::get< I >( inputs.types ) ) >;
        if ( value_mask & ( 1 << I ) ) {
            const ValueT value { *reinterpret_cast< const ValueT* >( data ) };
            const std::string value_str { _formatVALUE( value, inputs.traits[I] ) };
            const std::string_view indent { _tabIndent( inputs.tab_ct ) };
            if ( _multiline ) {
                outputs->str += fmt::format( "{}{: <{}} = {}\n",
                                             indent,
                                             inputs.names[I],
                                             inputs.name_width,
                                             value_str );
            } else {
                outputs->str += fmt::format( "{}{}={}",
                                             outputs->bytes_parsed == 0 ? "" : ", ",
                                             inputs.names[I], value_str );
            }
            outputs->bytes_parsed += _VALUE_ENCODING_SZ;
            _parseLISTofVALUE< I + 1, Args... >(
                value_mask, inputs, data + _VALUE_ENCODING_SZ, outputs );
        } else {
            _parseLISTofVALUE< I + 1, Args... >(
                value_mask, inputs, data, outputs );
        }
    }

    size_t _logClientInitiation(
        const Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logServerRefusal(
        Connection* conn, const uint8_t* data );
    size_t _logServerRequireFurtherAuthentication(
        Connection* conn, const uint8_t* data );
    _ParsingOutputs _parseLISTofFORMAT(
        const uint8_t* data, const uint32_t format_ct, const uint32_t tab_ct );
    _ParsingOutputs _parseLISTofSCREEN(
        const uint8_t* data, const uint32_t screen_ct, const uint32_t tab_ct );
    _ParsingOutputs _parseLISTofDEPTH(
        const uint8_t* data, const uint32_t depth_ct, const uint32_t tab_ct );
    _ParsingOutputs _parseLISTofVISUALTYPE(
        const uint8_t* data, const uint32_t vt_ct, const uint32_t tab_ct );
    size_t _logServerAcceptance(
        Connection* conn, const uint8_t* data );
    size_t _logServerResponse(
        Connection* conn, const uint8_t* data, const size_t sz );

    size_t _logClientPacket(
        Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerPacket(
        Connection* conn, uint8_t* data, const size_t sz );

    size_t _logServerError(
        Connection* conn, const uint8_t* data, const size_t sz );

    // TBD need data to be mutable for reply spoofing, eg QueryExtension
    size_t _logServerReply(
        Connection* conn, const uint8_t* data, const size_t sz );
    // TBD replies with modified contents
    template < typename RequestT >
    size_t _logServerReply(
        Connection* conn, uint8_t* data, const size_t sz );
    // TBD normal unmodified data
    template < typename RequestT >
    size_t _logServerReply(
        Connection* conn, const uint8_t* data, const size_t sz );

    size_t _logServerEvent(
        Connection* conn, const uint8_t* data, const size_t sz );
    enum class _EventFormat {
        NORMAL, SEND_EVENT
    };
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _EventFormat format = _EventFormat::NORMAL );
    template < typename EventT >
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data,
        const _EventFormat format = _EventFormat::NORMAL );


    size_t _logClientRequest(
        Connection* conn, const uint8_t* data, const size_t sz );

    template < typename RequestT,
               std::enable_if_t<
                   !( std::is_base_of_v<protocol::requests::impl::SimpleRequest, RequestT> ||
                      std::is_base_of_v<protocol::requests::impl::SimpleWindowRequest, RequestT> ||
                      std::is_base_of_v<protocol::requests::impl::ListFontsRequest, RequestT> ),
                   bool> = true >
    size_t _logClientRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    // GrabServer UngrabServer GetInputFocus QueryKeymap GetFontPath
    // ListExtensions GetKeyboardControl GetPointerControl GetScreenSaver
    // ListHosts GetPointerMapping GetModifierMapping

    size_t _logSimpleRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    template < typename RequestT,
               std::enable_if_t<
                   std::is_base_of_v<protocol::requests::impl::SimpleRequest, RequestT>,
                   bool> = true >
    inline size_t _logClientRequest(
        Connection* conn, const uint8_t* data, const size_t sz ) {
        return _logSimpleRequest( conn, data, sz );
    }

    // GetWindowAttributes DestroyWindow DestroySubwindows MapWindow
    // MapSubwindows UnmapWindow UnmapSubwindows QueryTree ListProperties
    // QueryPointer ListInstalledColormaps
    size_t _logSimpleWindowRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    template < typename RequestT,
               std::enable_if_t<
                   std::is_base_of_v<protocol::requests::impl::SimpleWindowRequest, RequestT>,
                   bool> = true >
    inline size_t _logClientRequest(
        Connection* conn, const uint8_t* data, const size_t sz ) {
        return _logSimpleWindowRequest( conn, data, sz );
    }

    // ListFonts ListFontsWithInfo
    size_t _logListFontsRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    template < typename RequestT,
               std::enable_if_t<
                   std::is_base_of_v<protocol::requests::impl::ListFontsRequest, RequestT>,
                   bool> = true >
    inline size_t _logClientRequest(
        Connection* conn, const uint8_t* data, const size_t sz ) {
        return _logListFontsRequest( conn, data, sz );
    }

public:
    X11ProtocolParser() {}

    void importSettings(
        FILE* log_fs, const bool multiline,
        const bool verbose, const bool readwritedebug,
        const bool denyallextensions );
    size_t logClientPackets( Connection* conn );
    size_t logServerPackets( Connection* conn );
};

template <>
struct X11ProtocolParser::is_variable_enum_common_type< protocol::PIXMAP > :
    public std::true_type {};

template <>
struct X11ProtocolParser::is_variable_enum_common_type< protocol::WINDOW > :
    public std::true_type {};

template <>
struct X11ProtocolParser::is_variable_enum_common_type< protocol::VISUALID > :
    public std::true_type {};

template <>
struct X11ProtocolParser::is_variable_enum_common_type< protocol::COLORMAP > :
    public std::true_type {};


#endif  // X11PROTOCOLPARSER_HPP
