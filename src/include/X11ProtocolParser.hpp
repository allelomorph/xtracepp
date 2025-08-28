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
#include "protocol/connection_setup.hpp"


class X11ProtocolParser {
private:
    // TBD could be reference to track changes in server.settings, but settings
    //   not likely to change after initial parse of CLI
    Settings settings;

    // TBD formattig struct?
    // TBD would prefer const, maybe make parser ctor that takes Settings param?
    std::string_view _SEPARATOR { " " };  // "\n"  for multiline
    std::string_view _EQUALS    { "=" };  // " = " for multiline

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

    // TBD maybe move inside _Indentation
    static std::string_view
    _tabIndent( const uint8_t tab_ct );

    class _Indentation {
    private:
        static constexpr uint8_t   _TAB_SZ { 2 };  // in spaces/bytes
        static constexpr uint8_t   _MEMBER_TAB_OFFSET { 1 };
        static constexpr uint8_t   _NESTING_TAB_OFFSET { 1 };

        bool _default_multiline {};
        _Indentation(const uint8_t base_tab_ct_, const bool default_multiline_,
                     const bool multiline_ ) :
            _default_multiline( default_multiline_ ),
            multiline( multiline_ ),
            base_tab_ct( base_tab_ct_ ),
            enclosure(
                !multiline_ ? "" : _tabIndent( base_tab_ct ) ),
            member(
                !multiline_ ? "" : _tabIndent( base_tab_ct + _MEMBER_TAB_OFFSET ) ) {}

        std::string_view
        _tabIndent( const uint8_t tab_ct );

    public:
        enum { SINGLELINE, MULTILINE, UNINITIALIZED };
        // TBD members cannot be const while using _Indentation(_Indentation&&),
        //   see importSettings
        bool                 multiline {};
        uint8_t              base_tab_ct {};
        std::string_view     enclosure {};
        std::string_view     member {};

        _Indentation() = delete;
        _Indentation(const uint8_t base_tab_ct_, const bool multiline_ ) :
            _default_multiline( multiline_ ),
            multiline( multiline_ ),
            base_tab_ct( base_tab_ct_ ),
            enclosure(
                !multiline_ ? "" : _tabIndent( base_tab_ct ) ),
            member(
                !multiline_ ? "" : _tabIndent( base_tab_ct + _MEMBER_TAB_OFFSET ) ) {}

        inline _Indentation
        nested( const int multiline_ = UNINITIALIZED ) const {
            return _Indentation (
                base_tab_ct + _NESTING_TAB_OFFSET,
                _default_multiline,
                multiline_ >= UNINITIALIZED ? _default_multiline : multiline_ );
        }
    };
    // TBD can only be const if set in parser ctor after server gets settings
    _Indentation _BASE_INDENTS { 0, settings.multiline };

    // "where E is some expression, and pad(E) is the number of bytes needed to
    //   round E up to a multiple of four."
    static constexpr size_t _ALIGN { 4 };
    inline size_t _padToAlignment( const size_t n, const size_t align ) {
        return n + ( ( align - ( n % align ) ) % align );
    };
    inline size_t _pad( const size_t n ) {
        return _padToAlignment( n, _ALIGN );
    }
    inline size_t _alignedUnits( const size_t sz ) {
        return sz / _ALIGN;
    }

    template < typename ScalarT,
               std::enable_if_t<std::is_scalar_v<ScalarT>, bool> = true >
    inline constexpr size_t _fmtHexWidth( const ScalarT val ) {
        // fmt counts "0x" as part of width when using '#'
        return ( sizeof( val ) * 2 ) + sizeof( "0x" );
    }

    struct _IndexRange {
        static constexpr uint32_t UNINIT {
            std::numeric_limits< uint32_t >::max() };
        uint32_t min { UNINIT };
        uint32_t max { UNINIT };

        _IndexRange() {}
        _IndexRange( const uint32_t min_, const uint32_t max_ ) :
            min( min_ ), max( max_ ) {}
    };

    // TBD using this as a default for name vector references prevents calling
    //   std::vector{} every time
    // TBD consider passing pointer instead of reference to allow for null
    //   checks (use std::optional?) and have lighter weight default ctor
    inline static const std::vector<std::string_view> _NO_NAMES {};

    // TBD [u]intXX_t CARDXX INTXX
    template < typename IntegerT,
               std::enable_if_t< std::is_integral_v< IntegerT >, bool > = true >
    std::string _formatInteger(
        const IntegerT int_,
        const std::vector< std::string_view >& enum_names = _NO_NAMES,
        _IndexRange name_index_range = {} ) {

        std::string name_str {};
        if ( !enum_names.empty() ) {
            assert( int_ != _IndexRange::UNINIT );
            if ( name_index_range.min == _IndexRange::UNINIT )
                name_index_range.min = 0;
            if ( name_index_range.max == _IndexRange::UNINIT )
                name_index_range.max = enum_names.size() - 1;
            if ( int_ >= name_index_range.min &&
                 int_ <= name_index_range.max ) {
                name_str = enum_names[ int_ ];
            }
        }
        if ( settings.verbose ) {
            const std::string hex_str {
                fmt::format( "{:#0{}x}", std::make_unsigned_t< IntegerT >( int_ ),
                             _fmtHexWidth( int_ ) ) };
            return name_str.empty() ? hex_str :
                fmt::format( "{}({})", hex_str, name_str );
        }
        return name_str.empty() ? fmt::format( "{:d}", int_ ) : name_str;
    }

    template < typename MaskT,
               std::enable_if_t< std::is_integral_v< MaskT >, bool > = true >
    std::string _formatBitmask(
        const MaskT mask,
        const std::vector< std::string_view >& flag_names = _NO_NAMES,
        _IndexRange name_index_range = {} ) {

        // TBD cannot use is_unsigned in SFINAE due to parallel call with
        //   _formatInteger inside _formatVALUE
        assert( std::is_unsigned_v< MaskT > );

        std::string hex_str { fmt::format( "{:#0{}x}", mask,
                                           _fmtHexWidth( mask ) ) };
        std::string flag_str;
        if ( !flag_names.empty() ) {
            assert( mask != _IndexRange::UNINIT );
            if ( name_index_range.min == _IndexRange::UNINIT )
                name_index_range.min = 0;
            if ( name_index_range.max == _IndexRange::UNINIT )
                name_index_range.max = flag_names.size() - 1;
            for ( uint32_t i { name_index_range.min }, end_i { name_index_range.max };
                  i <= end_i; ++i ) {
                if ( mask & ( 1 << i ) ) {
                    flag_str.append( flag_str.empty() ? "" : "," );
                    flag_str.append( flag_names[i] );
                }
            }
        }
        if ( settings.verbose ) {
            return flag_str.empty() ? hex_str :
                fmt::format( "{}({})", hex_str, flag_str );
        }
        return flag_str.empty() ? hex_str : flag_str;
    }

    template < typename ProtocolT >
    std::string
    _formatProtocolType(
        const ProtocolT value,
        const std::vector<std::string_view>& enum_names = _NO_NAMES );

    template < typename ProtocolT >
    std::string
    _formatProtocolType(
        const ProtocolT value, const _Indentation& indents );

    struct _ParsingOutputs {
        std::string str       {};
        uint32_t bytes_parsed {};
    };

    template < typename ProtocolT >
    _ParsingOutputs
    _parseProtocolType( const uint8_t* data, const size_t sz,
                        const _Indentation& indents );

    template < typename ProtocolT >
    struct _is_textitem_type :
        public std::integral_constant<
        bool,
        std::is_same_v< ProtocolT, protocol::requests::PolyText8::TEXTITEM8 > ||
        std::is_same_v< ProtocolT, protocol::requests::PolyText16::TEXTITEM16 > > {};

    template < typename ProtocolT >
    struct _is_variable_length_protocol_type :
        public std::integral_constant<
        bool,
        _is_textitem_type< ProtocolT >::value ||
        std::is_same_v< ProtocolT, protocol::STR > ||
        std::is_same_v< ProtocolT, protocol::HOST > ||
        std::is_same_v< ProtocolT, protocol::connection_setup::ServerAcceptance::SCREEN > ||
        std::is_same_v< ProtocolT, protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH > > {};

    // TBD create SFINAE filter to allow _parseLISTof<> to enforce always printing certain
    //   list member types as singleline
    // TBO OR we could just hardcode it into each template specialization of _parse|formatProtocolType
    // template < typename ProtocolT >
    // struct _is_protocol_type_always_formatted_singleline :
    //     public std::integral_constant<
    //     bool,
    //     std::is_same_v< ProtocolT, void/*protocol::*/ > > {};

    // TBD for LISTs that have no length provided eg PolyText8|16
    template < typename ProtocolT,
               std::enable_if_t<
                   _is_textitem_type< ProtocolT >::value, bool> = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz,
                  const _Indentation& indents ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT::Encoding )  );

        _ParsingOutputs outputs;
        outputs.str += '[';
        while ( _pad( outputs.bytes_parsed ) < sz ) {
            const _ParsingOutputs member {
                _parseProtocolType< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    indents.nested() ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", _SEPARATOR, indents.member, member.str );
        }
        outputs.str += fmt::format( "{}{}]",
                                    outputs.bytes_parsed == 0 ? "" : _SEPARATOR,
                                    indents.enclosure );
        return outputs;
    }

    // TBD most other LISTs should have length provided
    template < typename ProtocolT,
               std::enable_if_t<
                   !_is_textitem_type< ProtocolT >::value, bool> = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t n,
                  const _Indentation& indents ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT::Encoding )  );

        _ParsingOutputs outputs;
        outputs.str += '[';
        for ( uint16_t i {}; i < n; ++i ) {
            _ParsingOutputs member;
            if ( _is_variable_length_protocol_type< ProtocolT >::value ) {
                member = _parseProtocolType< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    indents.nested() );
            } else {
                // TBD would we ever need to pass enum/flag names?
                // TBD _formatProtocolType always singleline for now
                member = _formatProtocolType(
                    *reinterpret_cast< const ProtocolT* >(
                        data + outputs.bytes_parsed ) );
            }
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", _SEPARATOR, indents.member, member.str );
        }
        outputs.str += fmt::format( "{}{}]",
                                    n == 0 ? "" : _SEPARATOR,
                                    indents.enclosure );
        return outputs;
    }

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
    _parseLISTofATOM( const uint8_t* data, const uint16_t n );
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
    _parseLISTofFONTPROP( const uint8_t* data, const uint16_t n );
    // TBD CHARINFO is a shared between two requests
    _ParsingOutputs
    _parseLISTofCHARINFO( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofCOLORMAP( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofRGB( const uint8_t* data, const uint16_t n );
    _ParsingOutputs
    _parseLISTofHOST( const uint8_t* data, const uint16_t n );

    struct _VALUETraits {
        // TBD cannot have union of plain references?
        //   - https://stackoverflow.com/questions/38691282/use-of-union-with-reference
        // union {
        //     const std::vector< std::string_view >& enum_names {};
        //     const std::vector< std::string_view >& flag_names;
        // };
        const std::vector< std::string_view >& enum_names { _NO_NAMES };
        const std::vector< std::string_view >& flag_names { enum_names };
        _IndexRange name_index_range {};
        enum { IS_NOT_BITMASK, IS_BITMASK };
        const bool is_mask { IS_NOT_BITMASK };

        _VALUETraits() {}
        _VALUETraits( const std::vector<std::string_view>& names_,
                      const _IndexRange name_index_range_ = {},
            const bool is_mask_ = IS_NOT_BITMASK ) :
            enum_names( names_ ), flag_names( enum_names ),
            is_mask( is_mask_ ), name_index_range( name_index_range_ ) {

            if ( !enum_names.empty() ) {
                if ( name_index_range_.min == _IndexRange::UNINIT )
                    name_index_range.min = 0;
                if ( name_index_range_.max == _IndexRange::UNINIT )
                    name_index_range.max = enum_names.size() - 1;
                assert( name_index_range.min <= name_index_range.max );
            }
        }
    };

    // TBD put mask in parsing inputs, that way we can calc the name_width of
    //   widest name _used_, not just overall
    // TBD for that matter we could just put data and sz in there as well
    template < typename TupleT >
    struct _LISTofVALUEParsingInputs {
        const uint32_t value_mask;
        const TupleT& types;
        const std::vector< std::string_view >& names;
        const std::vector< _VALUETraits >& traits;
        const _Indentation indents;
        size_t name_width {};

        _LISTofVALUEParsingInputs() = delete;
        _LISTofVALUEParsingInputs(
            const uint32_t value_mask_,
            const TupleT& types_,
            const std::vector< std::string_view >& names_,
            const std::vector< _VALUETraits >& traits_,
            const _Indentation indents_ ) :
            value_mask( value_mask_ ), types( types_ ), names( names_ ),
            traits( traits_ ), indents( indents_ ) {
            assert( std::tuple_size< TupleT >{} == names.size() &&
                    names.size() == traits.size() );
            if ( indents.multiline ) {
                for ( uint32_t i {}, end_i ( names.size() - 1 ); i <= end_i; ++i ) {
                    if ( value_mask & ( 1 << i ) ) {
                        name_width = std::max( name_width, names[i].size() );
                    }
                }
            }
        }
    };

    // TBD [u]intXX_t CARDXX INTXX
    template < typename ValueT,
               std::enable_if_t< std::is_integral_v< ValueT >, bool > = true >
    inline std::string
    _formatVALUE(
        const ValueT value, const _VALUETraits& traits ) {
        return traits.is_mask ?
            _formatBitmask( value, traits.enum_names, traits.name_index_range ) :
            _formatInteger( value, traits.flag_names, traits.name_index_range );
    }

    // PIXMAP WINDOW VISUALID COLORMAP  // TBD these don't need to pass names, but no need for overload due to default param
    // TIMESTAMP CURSOR ATOM FONT BITGRAVITY WINGRAVITY BOOL SETofEVENT SETofPOINTEREVENT SETofDEVICEEVENT KEYCODE BUTTON SETofKEYMASK SETofKEYBUTMASK POINT RECTANGLE ARC
    template < typename ValueT,
               std::enable_if_t< !std::is_integral_v< ValueT >, bool > = true >
    inline std::string
    _formatVALUE(
        const ValueT value, const _VALUETraits& traits ) {
        return _formatProtocolType( value, traits.enum_names );
    }

    // tuple iteration allows for run time taversal of heterogeneous list of types
    // TBD all this may not be necessary - could we just parse all as uint32_t and cast as necessary?
    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args,
              std::enable_if_t< I == sizeof...( Args ), bool > = true >
    void _parseLISTofVALUE(
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        const uint8_t*/* data*/, _ParsingOutputs* outputs ) {
        if ( inputs.value_mask != 0 ) {
            outputs->str += inputs.indents.enclosure;
        }
        outputs->str += ']';
    }

    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args,
              std::enable_if_t< I < sizeof...( Args ), bool > = true >
    void _parseLISTofVALUE(
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        const uint8_t* data, _ParsingOutputs* outputs ) {
        if ( I == 0 ) {
            outputs->str += '[';
            if ( inputs.value_mask != 0 ) {
                outputs->str += _SEPARATOR;
            }
        }
        if ( inputs.value_mask & ( 1 << I ) == 0 ) {
            return _parseLISTofVALUE< I + 1, Args... >( inputs, data, outputs );
        }
        using ValueT = std::remove_reference_t<
            decltype( std::get< I >( inputs.types ) ) >;
        const ValueT value { *reinterpret_cast< const ValueT* >( data ) };
        outputs->bytes_parsed += sizeof( protocol::VALUE );

        outputs->str += fmt::format(
            "{}{: <{}}{}{}{}",
            inputs.indents.member, inputs.names[I], inputs.name_width, _EQUALS,
            _formatVALUE( value, inputs.traits[I] ), _SEPARATOR );

        return _parseLISTofVALUE< I + 1, Args... >(
            inputs, data + sizeof( protocol::VALUE ), outputs );
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
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Indentation& indents );
    template < typename EventT >
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Indentation& indents );


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
        const Settings& settings );
    size_t logClientPackets( Connection* conn );
    size_t logServerPackets( Connection* conn );

};


#endif  // X11PROTOCOLPARSER_HPP
