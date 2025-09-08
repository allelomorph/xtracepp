#ifndef X11PROTOCOLPARSER_HPP
#define X11PROTOCOLPARSER_HPP


#include <tuple>  // tuple_sz
#include <string>
#include <string_view>
#include <type_traits>  // enable_if_t remove_reference_t
#include <vector>
#include <algorithm>  // max
#include <limits>
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


// TBD forward declare here to allow friending
class ProxyX11Server;

class X11ProtocolParser {
private:
    // TBD could be reference to track changes in server.settings, but settings
    //   not likely to change after initial parse of CLI
    Settings settings;

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
    std::unordered_map<uint32_t, std::string> _nonseq_interned_atoms;
    // TBD prefer prefetched atoms when option is turned on
    std::vector<std::string> _seq_interned_atoms;

    // only atom string known at InternAtom request parsing
    void
    _stashAtom( const _StashedAtomID sa_id, const std::string_view atom_str );
    // when parsing InternAtom reply, then string and ATOM can be joined
    void
    _internStashedAtom( const _StashedAtomID sa_id, const protocol::ATOM atom );

    class _Whitespace {
    private:
        static constexpr uint8_t   _TAB_SZ { 2 };  // in spaces/bytes
        static constexpr uint8_t   _MEMBER_TAB_OFFSET { 1 };
        static constexpr uint8_t   _NESTING_TAB_OFFSET { 1 };

        static constexpr std::string_view _SL_EQUALS    { "=" };
        static constexpr std::string_view _SL_SEPARATOR { " " };
        static constexpr std::string_view _ML_EQUALS    { " = " };
        static constexpr std::string_view _ML_SEPARATOR { "\n" };

        bool _default_multiline {};
        _Whitespace(const uint8_t base_tab_ct_, const bool default_multiline_,
                     const bool multiline_ ) :
            _default_multiline( default_multiline_ ),
            multiline( multiline_ ),
            base_tab_ct( base_tab_ct_ ),
            encl_indent(
                !multiline_ ? "" : _tabIndent( base_tab_ct ) ),
            memb_indent(
                !multiline_ ? "" : _tabIndent( base_tab_ct + _MEMBER_TAB_OFFSET ) ),
            equals( multiline_ ? _ML_EQUALS : _SL_EQUALS ),
            separator( multiline_ ? _ML_SEPARATOR : _SL_SEPARATOR ) {}

        std::string_view
        _tabIndent( const uint8_t tab_ct );

    public:
        enum { SINGLELINE, MULTILINE, UNDEFINED };
        // TBD members cannot be const while using _Whitespace(_Whitespace&&),
        //   see importSettings
        bool                multiline {};
        uint8_t             base_tab_ct {};
        std::string_view    encl_indent {};
        std::string_view    memb_indent {};
        std::string_view    equals;     // TBD would prefer const& if not using copy ctor
        std::string_view    separator;  // TBD would prefer const& if not using copy ctor

        _Whitespace() = delete;
        _Whitespace(const uint8_t base_tab_ct_, const bool multiline_ ) :
            _default_multiline( multiline_ ),
            multiline( multiline_ ),
            base_tab_ct( base_tab_ct_ ),
            encl_indent(
                !multiline_ ? "" : _tabIndent( base_tab_ct ) ),
            memb_indent(
                !multiline_ ? "" : _tabIndent( base_tab_ct + _MEMBER_TAB_OFFSET ) ),
            equals( multiline_ ? _ML_EQUALS : _SL_EQUALS ),
            separator( multiline_ ? _ML_SEPARATOR : _SL_SEPARATOR ) {}

        inline _Whitespace
        nested( const int multiline_ = UNDEFINED ) const {
            return _Whitespace (
                base_tab_ct + _NESTING_TAB_OFFSET,
                _default_multiline,
                // TBD can nest singleline in multiline but not reverse
                multiline_ == SINGLELINE ? multiline_ : _default_multiline );
        }
    };
    // TBD can only be const if set in parser ctor after server gets settings
    _Whitespace _ROOT_WS { 0, settings.multiline };

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
        return ( sizeof( val ) * 2 ) + ( sizeof( "0x" ) - 1 );
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
        const ProtocolT value, const _Whitespace& indents );

    struct _ParsingOutputs {
        std::string str       {};
        uint32_t bytes_parsed {};
    };

    // TBD SFINAE here is less extensible and grows onerous, maybe use empty parent classes
    //   in protocol::impl to select categories of protocol types
    template < typename ProtocolT >
    struct _is_protocol_integer_alias :
        public std::is_integral< ProtocolT > {};
    template < typename ProtocolT >
    static constexpr bool _is_protocol_integer_alias_v =
        _is_protocol_integer_alias< ProtocolT >::value;

    template < typename ProtocolT >
    struct _is_textitem_protocol_type :
        public std::integral_constant<
        bool,
        std::is_same_v< ProtocolT, protocol::requests::PolyText8::TEXTITEM8 > ||
        std::is_same_v< ProtocolT, protocol::requests::PolyText16::TEXTITEM16 > > {};
    template < typename ProtocolT >
    static constexpr bool _is_textitem_protocol_type_v = _is_textitem_protocol_type< ProtocolT >::value;

    template < typename ProtocolT >
    struct _is_variable_length_struct_protocol_type :
        public std::integral_constant<
        bool,
        _is_textitem_protocol_type< ProtocolT >::value ||
        std::is_same_v< ProtocolT, protocol::STR > ||
        std::is_same_v< ProtocolT, protocol::HOST > ||
        std::is_same_v< ProtocolT, protocol::connection_setup::ServerAcceptance::SCREEN > ||
        std::is_same_v< ProtocolT, protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH > > {};
    template < typename ProtocolT >
    static constexpr bool _is_variable_length_struct_protocol_type_v =
        _is_variable_length_struct_protocol_type< ProtocolT >::value;

    template < typename ProtocolT >
    struct _is_fixed_length_struct_protocol_type :
        public std::integral_constant<
        bool,
        std::is_same_v< ProtocolT, protocol::CHAR2B > ||
        std::is_same_v< ProtocolT, protocol::POINT > ||
        std::is_same_v< ProtocolT, protocol::RECTANGLE > ||
        std::is_same_v< ProtocolT, protocol::ARC > ||
        std::is_same_v< ProtocolT, protocol::connection_setup::ServerAcceptance::FORMAT > ||
        std::is_same_v< ProtocolT, protocol::connection_setup::ServerAcceptance::SCREEN::DEPTH::VISUALTYPE > ||
        std::is_same_v< ProtocolT, protocol::requests::GetMotionEvents::TIMECOORD > ||
        std::is_same_v< ProtocolT, protocol::requests::QueryFont::FONTPROP > ||
        std::is_same_v< ProtocolT, protocol::requests::QueryFont::CHARINFO > ||
        std::is_same_v< ProtocolT, protocol::requests::PolySegment::SEGMENT > ||
        std::is_same_v< ProtocolT, protocol::requests::StoreColors::COLORITEM > ||
        std::is_same_v< ProtocolT, protocol::requests::QueryColors::RGB > > {};
    template < typename ProtocolT >
    static constexpr bool _is_fixed_length_struct_protocol_type_v =
        _is_fixed_length_struct_protocol_type< ProtocolT >::value;

    template < typename ProtocolT >
    struct _is_integer_protocol_type :
        public std::integral_constant<
        bool,
        !_is_protocol_integer_alias_v< ProtocolT > &&
        !_is_fixed_length_struct_protocol_type_v< ProtocolT > &&
        !_is_variable_length_struct_protocol_type_v< ProtocolT > > {};
    template < typename ProtocolT >
    static constexpr bool _is_integer_protocol_type_v =
        _is_integer_protocol_type< ProtocolT >::value;

    // TBD individual template specializations in source
    template < typename ProtocolT,
               std::enable_if_t<
                   _is_variable_length_struct_protocol_type_v< ProtocolT >, bool > = true>
    _ParsingOutputs
    _parseProtocolType( const uint8_t* data, const size_t sz,
                        const _Whitespace& ws );

    // TBD wrapper mainly for use with _parseLISTof<>
    template < typename ProtocolT,
               std::enable_if_t<
                   _is_fixed_length_struct_protocol_type_v< ProtocolT > , bool > = true>
    _ParsingOutputs
    _parseProtocolType( const uint8_t* data, const size_t sz,
                        const _Whitespace& ws ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        return { _formatProtocolType( *reinterpret_cast< const ProtocolT* >( data ),
                                      ws ),
                 sizeof( ProtocolT ) };
    }

    // TBD wrapper mainly for use with _parseLISTof<>
    template < typename ProtocolT,
               std::enable_if_t<
                   _is_protocol_integer_alias_v< ProtocolT >, bool > = true >
    _ParsingOutputs
    _parseProtocolType(
        const uint8_t* data, const size_t sz,
        const std::vector< std::string_view >& enum_names = _NO_NAMES ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        return { _formatInteger( *reinterpret_cast< const ProtocolT* >( data ),
                                 enum_names ),
                 sizeof( ProtocolT ) };
    }

    // TBD wrapper mainly for use with _parseLISTof<>
    // all other types, eg ATOM TIMESTAMP WINDOW
    template < typename ProtocolT,
               std::enable_if_t< _is_integer_protocol_type_v< ProtocolT >, bool > = true>
    _ParsingOutputs
    _parseProtocolType(
        const uint8_t* data, const size_t sz,
        const std::vector< std::string_view >& enum_names = _NO_NAMES ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        return { _formatProtocolType( *reinterpret_cast< const ProtocolT* >( data ),
                                      enum_names ),
                 sizeof( ProtocolT ) };
    }

    // TBD for LISTs that have no length provided eg PolyText8|16
    template < typename MemberT,
               std::enable_if_t< _is_textitem_protocol_type_v< MemberT >, bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz,
                  const _Whitespace& ws,
                  const int members_multiline = _Whitespace::UNDEFINED ) {
        assert( data != nullptr );
        // assert( sz >= sizeof( MemberT::Encoding ) ); // TBD may be empty list

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    sz == 0 ? "" : ws.separator );
        while ( _pad( outputs.bytes_parsed ) < sz ) {
            const _ParsingOutputs member {
                _parseProtocolType< MemberT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    ws.nested( members_multiline ) ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", outputs.bytes_parsed == 0 ? "" : ws.encl_indent );
        return outputs;
    }

    template < typename ProtocolT,
               std::enable_if_t<
                   _is_fixed_length_struct_protocol_type_v< ProtocolT > ||
                   _is_variable_length_struct_protocol_type_v< ProtocolT >, bool > = true>
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t n,
                  const _Whitespace& ws,
                  const int members_multiline = _Whitespace::UNDEFINED ) {
        assert( data != nullptr );
        // assert( sz >= sizeof( ProtocolT::Encoding ) ); // TBD may be empty list

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    n == 0 ? "" : ws.separator );
        for ( uint16_t i {}; i < n; ++i ) {
            const _ParsingOutputs member {
                _parseProtocolType< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    ws.nested( members_multiline ) ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", n == 0 ? "" : ws.encl_indent );
        return outputs;
    }

    template < typename ProtocolT,
               std::enable_if_t<
                   _is_integer_protocol_type_v< ProtocolT > ||
                   _is_protocol_integer_alias_v< ProtocolT >, bool > = true>
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t n,
                  const _Whitespace& ws,
                  const std::vector< std::string_view >& enum_names = _NO_NAMES ) {
        assert( data != nullptr );
        // assert( sz >= sizeof( ProtocolT::Encoding ) ); // TBD may be empty list

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    n == 0 ? "" : ws.separator );
        for ( uint16_t i {}; i < n; ++i ) {
            const _ParsingOutputs member {
                _parseProtocolType< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    enum_names ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", n == 0 ? "" : ws.encl_indent );
        return outputs;
    }

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
        const _Whitespace ws;
        size_t name_width {};

        _LISTofVALUEParsingInputs() = delete;
        _LISTofVALUEParsingInputs(
            const uint32_t value_mask_,
            const TupleT& types_,
            const std::vector< std::string_view >& names_,
            const std::vector< _VALUETraits >& traits_,
            const _Whitespace ws_ ) :
            value_mask( value_mask_ ), types( types_ ), names( names_ ),
            traits( traits_ ), ws( ws_ ) {
            assert( std::tuple_size< TupleT >{} == names.size() &&
                    names.size() == traits.size() );
            if ( ws.multiline ) {
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
            outputs->str += inputs.ws.encl_indent;
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
                outputs->str += inputs.ws.separator;
            }
        }
        if ( ( inputs.value_mask & ( 1 << I ) ) == 0 ) {
            return _parseLISTofVALUE< I + 1, Args... >( inputs, data, outputs );
        }
        using ValueT = std::remove_reference_t<
            decltype( std::get< I >( inputs.types ) ) >;
        const ValueT value { *reinterpret_cast< const ValueT* >( data ) };
        outputs->bytes_parsed += sizeof( protocol::VALUE );

        outputs->str += fmt::format(
            "{}{: <{}}{}{}{}",
            inputs.ws.memb_indent, inputs.names[I], inputs.name_width, inputs.ws.equals,
            _formatVALUE( value, inputs.traits[I] ), inputs.ws.separator );
        return _parseLISTofVALUE< I + 1, Args... >(
            inputs, data + sizeof( protocol::VALUE ), outputs );
    }

    size_t _logClientInitiation(
        const Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logServerRefusal(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logServerRequireFurtherAuthentication(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logServerAcceptance(
        Connection* conn, const uint8_t* data, const size_t sz );
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

    static constexpr uint8_t _GENERATED_EVENT_FLAG { 0x80 };
    static constexpr uint8_t _EVENT_CODE_MASK { uint8_t(~_GENERATED_EVENT_FLAG) };
    size_t _logServerEvent(
        Connection* conn, const uint8_t* data, const size_t sz );
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Whitespace& indents );
    template < typename EventT >
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Whitespace& indents );


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

    // TBD would prefer to only allow
    //   - ProxyX11Server::_authenticateServerConnection()
    //     - _pad(), _alignedUnits()
    //   - ProxyX11Server::_fetchCurrentServerTime()
    //     - _pad(), _alignedUnits()
    //   - ProxyX11Server::_fetchInternedAtoms()
    //     - _pad(), _alignedUnits(), _prefetched_interned_atoms
    //   but we can't make undefined class methods friends, and we can't define
    //   ProxyX11Server as it has a X11ProtocolParser member, which would create a
    //   circular include
    friend ProxyX11Server;

public:
    X11ProtocolParser() {}

    void importSettings(
        const Settings& settings );
    size_t logClientPackets( Connection* conn );
    size_t logServerPackets( Connection* conn );
};


#endif  // X11PROTOCOLPARSER_HPP
