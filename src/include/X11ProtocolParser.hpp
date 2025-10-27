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
#include "protocol/connection_setup.hpp"


class X11ProtocolParser {
private:
    ////// Encoding Memory Alignment

    // https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Syntactic_Conventions_b
    // "where E is some expression, and pad(E) is the number of bytes needed to
    //   round E up to a multiple of four."
    class _Alignment {
    private:
        static constexpr size_t _ALIGN { 4 };

    public:
        inline size_t pad( const size_t sz ) {
            return sz + ( ( _ALIGN - ( sz % _ALIGN ) ) % _ALIGN );
        }
        inline size_t units( const size_t sz ) {
            return pad( sz ) / _ALIGN;
        }
        inline size_t size( const size_t units ) {
            return units * _ALIGN;
        }
    };

    ////// Atom Internment

    struct _StashedStringID {
        uint32_t conn_id {};
        uint16_t seq_num {};

        bool operator==( const _StashedStringID& other ) const {
            return this->conn_id == other.conn_id &&
                this->seq_num == other.seq_num;
        }
        struct Hash {
            size_t operator()( const _StashedStringID& sa_id ) const noexcept {
                const size_t h1 { std::hash<uint32_t>{}( sa_id.conn_id ) };
                const size_t h2 { std::hash<uint16_t>{}( sa_id.seq_num ) };
                return h1 ^ ( h2 << 1 ); // or use boost::hash_combine
            }
        };
    };
    // Temporary storage of strings defined in requests that are then needed for
    //   operations in the corresponding reply (eg InternAtom and QueryExtension.)
    //   Indexed by combination of connection ID + sequence number of request.
    std::unordered_map< _StashedStringID, std::string_view,
                        _StashedStringID::Hash >
    _stashed_strings;

    // Internment of strings at least partially mirroring that of actual X
    //   server, indexed by ATOM. Should contain:
    //   - all protocol predefined atoms (1 "PRIMARY" - 68 "WM_TRANSIENT_FOR")
    //   - any atoms in InternAtom requests made by any client during main queue
    //   - optionally with --prefetchatoms, all already interned strings in the
    //     first contiguous range of ATOMs starting with 1
    std::unordered_map< uint32_t, std::string > _interned_atoms;

    // only atom string known at InternAtom request parsing
    void
    _stashString( const _StashedStringID ss_id, const std::string_view str );
    // when parsing InternAtom reply, then string and ATOM can be joined
    void
    _internStashedAtom( const _StashedStringID sa_id, const protocol::ATOM atom );

    ////// Non-data Text Formatting

public:
    // made accessible to ProxyX11Server for use in error messages
    static constexpr std::string_view CLIENT_TO_SERVER { "s<c" };
    static constexpr std::string_view SERVER_TO_CLIENT { "s>c" };

private:
    class _Whitespace {
    private:
        static constexpr uint8_t _TAB_SZ { 2 };  // in spaces/bytes
        static constexpr uint8_t _MEMBER_TAB_OFFSET  { 1 };
        static constexpr uint8_t _NESTING_TAB_OFFSET { 1 };

        static constexpr std::string_view _SL_EQUALS    { "=" };
        static constexpr std::string_view _SL_SEPARATOR { " " };
        static constexpr std::string_view _ML_EQUALS    { " = " };
        static constexpr std::string_view _ML_SEPARATOR { "\n" };

        bool _default_multiline {};

        _Whitespace( const uint8_t base_tab_ct_, const bool default_multiline_,
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

        inline std::string_view
        _tabIndent( const uint8_t tab_ct ) {
            static constexpr std::string_view WHITESPACE {
                "                                                           "
                "                                                           "
            };
            const size_t indent_sz ( tab_ct * _TAB_SZ );
            assert( indent_sz <= WHITESPACE.size() );
            return { WHITESPACE.data(), indent_sz };
        }

    public:
        bool             multiline   {};
        uint8_t          base_tab_ct {};
        std::string_view encl_indent;
        std::string_view memb_indent;
        std::string_view equals;
        std::string_view separator;

        _Whitespace() = delete;
        _Whitespace( const uint8_t base_tab_ct_, const bool multiline_ ) :
            _default_multiline( multiline_ ),
            multiline( multiline_ ),
            base_tab_ct( base_tab_ct_ ),
            encl_indent(
                !multiline_ ? "" : _tabIndent( base_tab_ct_ ) ),
            memb_indent(
                !multiline_ ? "" : _tabIndent( base_tab_ct_ + _MEMBER_TAB_OFFSET ) ),
            equals( multiline_ ? _ML_EQUALS : _SL_EQUALS ),
            separator( multiline_ ? _ML_SEPARATOR : _SL_SEPARATOR ) {}

        enum { DEFAULT, FORCE_SINGLELINE };
        inline _Whitespace
        nested( const bool force_singleline = DEFAULT ) const {
            return _Whitespace (
                base_tab_ct + _NESTING_TAB_OFFSET,
                _default_multiline,
                force_singleline == FORCE_SINGLELINE ?
                false : _default_multiline );
        }
    };
    _Whitespace _ROOT_WS { 0, settings.multiline };

    ////// Individual Data Field Formatting

    template< typename ScalarT,
              std::enable_if_t<std::is_scalar_v<ScalarT>, bool> = true >
    inline constexpr size_t _fmtHexWidth( const ScalarT val ) {
        // fmt counts "0x" as part of width when using '#'
        return ( sizeof( val ) * 2 ) + ( sizeof( "0x" ) - 1 );
    }

    class _EnumNameRange {
    public:
        enum class Bound { MIN, MAX };

    private:
        static constexpr uint32_t _UNINIT {
            std::numeric_limits< uint32_t >::max() };
        inline static const std::vector< std::string_view > _NO_NAMES {};

        _EnumNameRange
        _ctorHelper( const std::vector< std::string_view >& names_,
                     const Bound bound, const uint32_t value ) {
            if ( bound == Bound::MIN )
                return _EnumNameRange( names_, value, names_.size() - 1 );
            assert( bound == Bound::MAX );
            return _EnumNameRange( names_, 0, value );
        }
        _EnumNameRange
        _ctorHelper( const std::vector< std::string_view >& names_ ) {
            return _EnumNameRange( names_, 0, names_.size() - 1 );
        }

    public:
        const std::vector< std::string_view >& names { _NO_NAMES };
        uint32_t min { _UNINIT };
        uint32_t max { _UNINIT };

        _EnumNameRange() {}
        _EnumNameRange( const std::vector< std::string_view >& names_,
                        const uint32_t min_, const uint32_t max_ ) :
            names( names_ ), min( min_ ), max( max_ ) {
            assert( !names.empty() );
            assert( max < names_.size() );
            assert( min <= max );
        }
        _EnumNameRange( const std::vector< std::string_view >& names_,
                        const Bound bound, const uint32_t value ) :
            _EnumNameRange( _ctorHelper( names_, bound, value ) ) {}
        _EnumNameRange( const std::vector< std::string_view >& names_ ) :
            _EnumNameRange( _ctorHelper( names_ ) ) {}

        template< typename IntT,
                  std::enable_if_t< std::is_integral_v< IntT >, bool > = true >
        inline bool in( const IntT val ) const {
            if ( std::is_signed_v< IntT > && val < 0 )
                return false;
            if ( names.empty() )
                return false;
            return ( val >= min && val <= max );
        }
        template< typename IntT,
                  std::enable_if_t< std::is_integral_v< IntT >, bool > = true >
        inline std::string_view at( const IntT val ) const {
            return in( val ) ? names[ val ] : "";
        }
        inline bool empty() const {
            return names.empty();
        }
    };

    struct _ValueTraits {
        _EnumNameRange name_range {};
        bool           bitmask    { false };

        enum { NOT_BITMASK, BITMASK };
        _ValueTraits() {}
        _ValueTraits( const _EnumNameRange name_range_,
                      const bool bitmask_ = NOT_BITMASK ) :
            name_range( name_range_ ), bitmask( bitmask_ ) {}
    };

    // fundamental integers (and protocol aliases thereof)
    template< typename IntT,
              std::enable_if_t< std::is_integral_v< IntT >, bool > = true >
    std::string _formatVariable( const IntT val,
                                 const _EnumNameRange name_range = {},
                                 const bool bitmask = _ValueTraits::NOT_BITMASK ) {
        const std::string hex_str {
            fmt::format( "{:#0{}x}", std::make_unsigned_t< IntT >( val ),
                         _fmtHexWidth( val ) ) };

        if ( bitmask ) {
            assert( std::is_unsigned_v< IntT > );
            std::string flag_str;
            if ( !name_range.empty() ) {
                for ( uint32_t i { name_range.min },
                          end_i { name_range.max }; i <= end_i; ++i ) {
                    if ( val & ( 1 << i ) ) {
                        flag_str.append( flag_str.empty() ? "" : "," );
                        flag_str.append( name_range.at( i ) );
                    }
                }
            }
            if ( flag_str.empty() )
                return hex_str;
            return settings.verbose ?
                fmt::format( "{}({})", hex_str, flag_str ) :
                flag_str;
        }

        if ( const std::string name_str { name_range.at( val ) };
             !name_str.empty() ) {
            return settings.verbose ?
                fmt::format( "{}({})", hex_str, name_str ) :
                name_str;
        }
        return settings.verbose ?
            fmt::format( "{}({:d})", hex_str, val ) :
            fmt::format( "{:d}", val );
    }

    // non-ATOM ResourceIds
    template< typename ResourceIdT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::ResourceId, ResourceIdT > &&
                  !std::is_same_v< protocol::ATOM, ResourceIdT >,
                  bool > = true >
    std::string _formatVariable( const ResourceIdT var,
                                 const _EnumNameRange name_range = {} ) {
        assert( ( var.data & ResourceIdT::ZERO_BITS ) == 0 );
        return _formatVariable( var.data, name_range );
    }

    // only ResourceId with unique definition
    std::string _formatVariable( const protocol::DRAWABLE var,
                                 const _EnumNameRange name_range = {} );
    // union of ResourceId
    std::string _formatVariable( const protocol::FONTABLE var,
                                 const _EnumNameRange name_range = {} );
    // union of ResourceId
    std::string _formatVariable( const protocol::ATOM var,
                                 const _EnumNameRange name_range = {} );

    // non-ResourceId protocol int types
    template< typename ProtocolIntT,
              std::enable_if_t<
                  ( std::is_base_of_v< protocol::impl::Integer, ProtocolIntT > &&
                    !std::is_base_of_v< protocol::impl::ResourceId, ProtocolIntT > ),
                  bool > = true >
    std::string _formatVariable( const ProtocolIntT var,
                                 const _EnumNameRange name_range = {} );

    // plain structs with no variable length suffixes
    template< typename ProtocolStructT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::Struct, ProtocolStructT > &&
                  !std::is_base_of_v<
                      protocol::impl::StructWithSuffixes, ProtocolStructT >,
                  bool > = true >
    std::string _formatVariable( const ProtocolStructT var,
                                 const _Whitespace& ws );

    ////// LISTof* Formatting

    struct _ParsingOutputs {
        std::string str       {};
        uint32_t bytes_parsed {};
    };

    // fundamental integers (and protocol aliases thereof)
    template< typename ProtocolT,
              std::enable_if_t< std::is_integral_v< ProtocolT >,
                                bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz,
                      const _ValueTraits traits = {} ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        const ProtocolT memb { *reinterpret_cast< const ProtocolT* >( data ) };
        return { _formatVariable( memb, traits.name_range, traits.bitmask ),
                 sizeof( ProtocolT ) };
    }

    // protocol integer types (eg ResourceId, Bitmask)
    template< typename ProtocolT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::Integer, ProtocolT >,
                  bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz,
                      const _ValueTraits traits = {} ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        const ProtocolT memb { *reinterpret_cast< const ProtocolT* >( data ) };
        return { _formatVariable( memb, traits.name_range ),
                 sizeof( ProtocolT ) };
    }

    // plain structs with no variable length suffixes
    template< typename ProtocolStructT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::Struct, ProtocolStructT > &&
                  !std::is_base_of_v<
                      protocol::impl::StructWithSuffixes, ProtocolStructT >,
                  bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz,
                      const _Whitespace& ws ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolStructT ) );

        const ProtocolStructT memb {
            *reinterpret_cast< const ProtocolStructT* >( data ) };
        return { _formatVariable( memb, ws ),
                 sizeof( ProtocolStructT ) };
    }

    // plain structs with variable length suffixes + TEXTITEM8|16
    template< typename ProtocolStructT,
              std::enable_if_t<
                  // union of StructWithSuffixes and Struct
                  std::is_same_v< ProtocolStructT,
                                  protocol::requests::PolyText8::TEXTITEM8 > ||
                  // union of StructWithSuffixes and Struct
                  std::is_same_v< ProtocolStructT,
                                  protocol::requests::PolyText16::TEXTITEM16 > ||
                  std::is_base_of_v<
                      protocol::impl::StructWithSuffixes, ProtocolStructT >,
                  bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz,
                      const _Whitespace& ws );

    // TBD for LISTs that have no length provided eg PolyText8|16
    template< typename ProtocolT,
              std::enable_if_t<
                  ( !std::is_integral_v< ProtocolT > &&
                    !std::is_base_of_v< protocol::impl::Integer, ProtocolT > ),
                  bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz,
                  const _Whitespace& ws,
                  const bool force_membs_singleline = _Whitespace::DEFAULT ) {
        assert( data != nullptr );

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    sz == 0 ? "" : ws.separator );
        while ( alignment.pad( outputs.bytes_parsed ) < sz ) {
            const _ParsingOutputs member {
                _parseListMember< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    ws.nested( force_membs_singleline ) ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", outputs.bytes_parsed == 0 ? "" : ws.encl_indent );
        return outputs;
    }

    template< typename ProtocolT,
              std::enable_if_t<
                  ( !std::is_integral_v< ProtocolT > &&
                    !std::is_base_of_v< protocol::impl::Integer, ProtocolT > ),
                  bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t memb_ct,
                  const _Whitespace& ws,
                  const bool force_membs_singleline = _Whitespace::DEFAULT ) {
        assert( data != nullptr );

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    memb_ct == 0 ? "" : ws.separator );
        for ( uint16_t i {}; i < memb_ct; ++i ) {
            const _ParsingOutputs member {
                _parseListMember< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    ws.nested( force_membs_singleline ) ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", memb_ct == 0 ? "" : ws.encl_indent );
        return outputs;
    }

    template< typename ProtocolT,
              std::enable_if_t<
                  std::is_integral_v< ProtocolT > ||
                  std::is_base_of_v< protocol::impl::Integer, ProtocolT >,
                  bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t memb_ct,
                  const _Whitespace& ws,
                  const _ValueTraits traits = {} ) {
        assert( data != nullptr );

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    memb_ct == 0 ? "" : ws.separator );
        for ( uint16_t i {}; i < memb_ct; ++i ) {
            const _ParsingOutputs member {
                _parseListMember< ProtocolT >(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    traits ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", memb_ct == 0 ? "" : ws.encl_indent );
        return outputs;
    }

    ////// LISTofVALUE Formatting

    template< typename TupleT >
    struct _LISTofVALUEParsingInputs {
        const uint8_t* data;
        const size_t sz;
        const uint32_t value_mask;
        const TupleT& types;
        const std::vector< std::string_view >& names;
        const std::vector< _ValueTraits >& traits;
        const _Whitespace ws;
        size_t memb_name_w {};

        _LISTofVALUEParsingInputs() = delete;
        _LISTofVALUEParsingInputs(
            const uint8_t* data_, const size_t sz_, const uint32_t value_mask_,
            const TupleT& types_, const std::vector< std::string_view >& names_,
            const std::vector< _ValueTraits >& traits_,
            const _Whitespace ws_ ) :
            data( data_ ), sz( sz_ ), value_mask( value_mask_ ),
            types( types_ ), names( names_ ), traits( traits_ ), ws( ws_ ) {

            assert( data != nullptr );
            assert( std::tuple_size< TupleT >{} == names.size() &&
                    names.size() == traits.size() );
            if ( ws.multiline ) {
                for ( uint32_t i {}, sz ( names.size() ); i < sz; ++i ) {
                    if ( value_mask & ( 1 << i ) ) {
                        memb_name_w = std::max( memb_name_w, names[ i ].size() );
                    }
                }
            }
        }
    };

    struct _LISTofVALUEParsingOutputs : public _ParsingOutputs {
        uint32_t value_ct {};
    };

    // tuple iteration allows for run time taversal of heterogeneous list of types
    // tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args,
              std::enable_if_t< I == sizeof...( Args ), bool > = true >
    void _parseLISTofVALUE(
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        _LISTofVALUEParsingOutputs* outputs ) {
        assert( outputs->bytes_parsed ==
                outputs->value_ct * sizeof( protocol::VALUE ) );
        if ( inputs.value_mask != 0 ) {
            outputs->str += inputs.ws.encl_indent;
        }
        outputs->str += ']';
    }

    // tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args,
              std::enable_if_t< I < sizeof...( Args ), bool > = true >
    void _parseLISTofVALUE(
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        _LISTofVALUEParsingOutputs* outputs ) {
        if ( I == 0 ) {
            outputs->str += '[';
            if ( inputs.value_mask != 0 ) {
                outputs->str += inputs.ws.separator;
            }
        }
        if ( ( inputs.value_mask & ( 1 << I ) ) == 0 ) {
            return _parseLISTofVALUE< I + 1, Args... >( inputs, outputs );
        }
        outputs->value_ct++;
        assert( inputs.sz - outputs->bytes_parsed >=
                sizeof( protocol::VALUE ) );
        using ValueT = std::remove_reference_t<
            decltype( std::get< I >( inputs.types ) ) >;
        const _ParsingOutputs member {
            _parseListMember< ValueT >(
                inputs.data + outputs->bytes_parsed,
                inputs.sz - outputs->bytes_parsed,
                inputs.traits[ I ] ) };
        // do not use member.bytes_parsed as all VALUEs are encoded as 4B
        outputs->bytes_parsed += sizeof( protocol::VALUE );

        outputs->str += fmt::format(
            "{}{: <{}}{}{}{}",
            inputs.ws.memb_indent, inputs.names[ I ], inputs.memb_name_w,
            inputs.ws.equals, member.str, inputs.ws.separator );
        return _parseLISTofVALUE< I + 1, Args... >( inputs, outputs );
    }

    ////// Logging

    size_t _logClientPacket(
            Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerPacket(
        Connection* conn, uint8_t* data, const size_t sz );

    size_t _logConnInitiation(
        const Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logConnRefusal(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logConnRequireFurtherAuthentication(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logConnAcceptance(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logConnResponse(
        Connection* conn, const uint8_t* data, const size_t sz );

    template< typename RequestT >
    _ParsingOutputs _parseRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logRequest(
        Connection* conn, const uint8_t* data, const size_t sz );

    template< typename RequestT >
    _ParsingOutputs _parseReply(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logReply(
        Connection* conn, const uint8_t* data, const size_t sz );

    template< typename EventT >
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Whitespace& indents );
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Whitespace& indents );
    size_t _logEvent(
        Connection* conn, const uint8_t* data, const size_t sz );

    template< typename ErrorT >
    _ParsingOutputs _parseError(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logError(
        Connection* conn, const uint8_t* data, const size_t sz );

public:
    _Alignment alignment;

    Settings settings;

    X11ProtocolParser() {}

    void importSettings( const Settings& settings_,
                         const std::vector< std::string >& fetched_atoms );
    size_t logClientPackets( Connection* conn );
    size_t logServerPackets( Connection* conn );
};


#endif  // X11PROTOCOLPARSER_HPP
