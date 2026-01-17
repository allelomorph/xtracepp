#ifndef X11PROTOCOLPARSER_HPP
#define X11PROTOCOLPARSER_HPP

/**
 * @file X11ProtocolParser.hpp
 */

#include <tuple>          // tuple_size
#include <string>
#include <string_view>
#include <type_traits>    // enable_if_t remove_reference_t
#include <vector>
#include <algorithm>      // max
#include <limits>
#include <unordered_map>

#include <cassert>

#include <stdio.h>        // stdout

#include <fmt/format.h>

#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/Message.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/common_types.hpp"
#include "protocol/requests.hpp"
#include "protocol/connection_setup.hpp"


/**
 * @brief Obscures `operator<` to prevent Doxygen (v1.9.8) parsing it as an
 *   opening caret in template parameters.
 */
#define _LESSTHAN <

/**
 * @brief Obscures `operator<=` to prevent Doxygen (v1.9.8) parsing it as an
 *   opening caret in template parameters.
 */
#define _LESSTHANOREQUALTO <=

/**
 * @brief Obscures [_parseListMember](#X11ProtocolParser::_parseListMember)
 *   template specialization syntax to prevent Doxygen (v1.9.8) parsing
 *   bug.
 * @warning When Doxygen (v1.9.8) parses a documented, templated function
 *   after it was defined in the same header, for (minimal) example:
 *   ```
 *   //\! @brief Text.
 *   //\!
 *   template< typename T >
 *   void _ppp() {}
 *   _ppp< U >();
 *   ```
 *   many random tokens (for example `const`, `static`, `size_t` in this header)
 *   are reference-linked to the first overload of that function.
 * @param type_ type to pass to template
 */
#define _PARSELISTMEMBER( type_ ) _parseListMember< type_ >

/**
 * @brief Parses server and client data packets sent encoded in the X11 protocol,
 *   logging them to an output file stream.
 */
class X11ProtocolParser {
private:
    /**
     * @brief Calculator for converting raw byte counts to and from [padded]
     *   lengths commonly used in encoding.
     * [padded]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Syntactic_Conventions_b
     */
    class _Alignment {
    private:
        /**
         * @brief Alignment size in bytes.
         */
        static constexpr size_t _ALIGN { protocol::Message::ALIGN };

    public:
        /**
         * @brief Pads a raw byte count to nearest larger multiple of 4.
         * @param sz raw byte count
         * @return sz padded to aligned size (nearest larger multiple of 4)
         */
        inline size_t pad( const size_t sz ) {
            return sz + ( ( _ALIGN - ( sz % _ALIGN ) ) % _ALIGN );
        }
        /**
         * @brief Converts a raw byte count to alignment unit count once padded.
         * @param sz raw byte count
         * @return padded sz in alignment units
         */
        inline size_t units( const size_t sz ) {
            return pad( sz ) / _ALIGN;
        }
        /**
         * @brief Converts alignment unit count to byte count.
         * @param units alignment unit count
         * @return units in bytes
         */
        inline size_t size( const size_t units ) {
            return units * _ALIGN;
        }
    };
    /**
     * @brief Bundles [connection](#Connection) ID and request sequence number
     *   to create unique server-wide ID for any request made.
     * @ingroup atom_internment
     */
    struct _StashedStringID {
        /**
         * @brief [Connection](#Connection) unique serial number.
         */
        uint32_t conn_id {};
        /**
         * @brief Request serial number, unique for a on a given
         *   [connection](#Connection).
         */
        uint16_t seq_num {};
        /**
         * @brief Equivalence operator to allow use as a hashmap key.
         */
        bool operator==( const _StashedStringID& other ) const {
            return this->conn_id == other.conn_id &&
                this->seq_num == other.seq_num;
        }
        /**
         * @brief Hashing functor to allow use as a hashmap key.
         */
        struct Hash {
            /**
             * @brief Function call operator to allow use as a functor.
             * @note Uses this [method] to combine hashes, [boost::hash_combine]
             *   also comes recommended
             * [method]: https://stackoverflow.com/questions/35985960/
             * [boost::hash_combine]: https://www.boost.org/doc/libs/latest/libs/container_hash/doc/html/hash.html#ref_hash_combine
             */
            size_t operator()( const _StashedStringID& sa_id ) const noexcept {
                const size_t h1 { std::hash<uint32_t>{}( sa_id.conn_id ) };
                const size_t h2 { std::hash<uint16_t>{}( sa_id.seq_num ) };
                return h1 ^ ( h2 << 1 );
            }
        };
    };
    /**
     * @brief Temporary storage for strings defined in requests that are then
     *  later needed for when parsing the corresponding reply (eg InternAtom
     *   and QueryExtension).
     * @ingroup atom_internment
     */
    std::unordered_map< _StashedStringID, std::string_view,
                        _StashedStringID::Hash >
    _stashed_strings;
    /**
     * @brief Temporarily store a string encoded in a request that will be
     *   needed when parsing its reply.
     * @param ss_id unique identifier combining connection id + request
     *   sequence number
     * @param str string to be stored
     * @ingroup atom_internment
     */
    void
    _stashString( const _StashedStringID ss_id, const std::string_view str );
    /**
     * @brief Internment of strings at least partially mirroring that of actual
     *   X server, indexed by ATOM.
     * Should contain:
     *   - all protocol predefined atoms (1 "PRIMARY" - 68 "WM_TRANSIENT_FOR")
     *   - any atoms in InternAtom requests made by any client during main queue
     *   - optionally with --prefetchatoms, all server interned strings in the
     *     first contiguous range of ATOMs starting with 1
     * @ingroup atom_internment
     */
    std::unordered_map< uint32_t, std::string > _interned_atoms;
    /**
     * @brief Moves string from temporary storage to local internment as atom.
     * @param sa_id unique identifier for combining connection id + request
     *   sequence number
     * @param atom ATOM XId to map string onto
     * @ingroup atom_internment
     */
    void
    _internStashedAtom( const _StashedStringID sa_id, const protocol::ATOM atom );

public:
    // CLIENT_TO_SERVER and SERVER_TO_CLIENT need to be accessible to
    //   ProxyX11Server for use in error messages
    /**
     * @brief Log formatting token indicating X server-bound packet.
     * @ingroup non_data_text_formatting
     */
    static constexpr std::string_view CLIENT_TO_SERVER { "s<c" };
    /**
     * @brief Log formatting token indicating client-bound packet.
     * @ingroup non_data_text_formatting
     */
    static constexpr std::string_view SERVER_TO_CLIENT { "s>c" };

private:
    /**
     * @brief Manages variable whitespace and formatting based on
     *   [verbosity](#Settings::verbose) and parentheses nesting depth.
     * @ingroup non_data_text_formatting
     */
    class _Whitespace {
    private:
        /**
         * @brief Tab size in space chars/bytes.
         */
        static constexpr uint8_t _TAB_SZ { 2 };
        /**
         * @brief Number of tabs a struct/list/array member should be indented
         *   from its brackets/parentheses.
         */
        static constexpr uint8_t _MEMBER_TAB_OFFSET  { 1 };
        /**
         * @brief Number of tabs a nested struct/list/array should be indented
         *   from its parent.
         */
        static constexpr uint8_t _NESTING_TAB_OFFSET { 1 };
        /**
         * @brief Equals sign token when in singleline mode
         *   ([multiline](#Settings::multiline) is `false`).
         */
        static constexpr std::string_view _SL_EQUALS    { "=" };
        /**
         * @brief General separator token when in singleline mode
         *   ([multiline](#Settings::multiline) is `false`).
         */
        static constexpr std::string_view _SL_SEPARATOR { " " };
        /**
         * @brief Equals sign token when in multiline mode
         *   ([multiline](#Settings::multiline) is `true`).
         */
        static constexpr std::string_view _ML_EQUALS    { " = " };
        /**
         * @brief General separator token when in multiline mode
         *   ([multiline](#Settings::multiline) is `true`).
         */
        static constexpr std::string_view _ML_SEPARATOR { "\n" };
        /**
         * @brief Records [multiline](#Settings::multiline) setting used in root
         *   nesting level, and by default in all other nesting levels.
         */
        bool _default_multiline {};
        /**
         * @brief Private ctor used to implement [nested](#nested).
         * @param base_tab_ct_ tab count at current nesting level
         * @param default_multiline_ multiline setting of nesting root
         * @param multiline_ (arbitrary) multiline setting of current nesting level
         */
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
        /**
         * @brief Generates indentation tokens.
         * @param tab_ct tab count of indent
         * @return indentation token
         */
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
        /**
         * @brief Multiline setting of current nesting level.
         */
        bool             multiline   {};
        /**
         * @brief Tab count at current nesting level.
         */
        uint8_t          base_tab_ct {};
        /**
         * @brief Indentation token for struct/list/array brackets/parentheses
         *   at current nesting level.
         */
        std::string_view encl_indent;
        /**
         * @brief Indentation token for struct/list/array members at current
         *   nesting level.
         */
        std::string_view memb_indent;
        /**
         * @brief Equals sign token.
         */
        std::string_view equals;
        /**
         * @brief General separator token.
         */
        std::string_view separator;
        _Whitespace() = delete;
        /**
         * @brief Primary ctor.
         * @param base_tab_ct_ tab count at current nesting level
         * @param multiline_ (arbitrary) multiline setting of current nesting level
         */
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
        /**
         * @brief Constants used to sweeten the syntax of [nested](#nested)
         *   parameter `force_singleline`.
         */
        enum { DEFAULT, FORCE_SINGLELINE };
        /**
         * @brief Create new _Whitespace at the next nesting level.
         * @param force_singleline whether to arbitrarily format next nesting
         *   level as singleline
         * @return new _Whitespace at the next nesting level
         */
        inline _Whitespace
        nested( const bool force_singleline = DEFAULT ) const {
            return _Whitespace (
                base_tab_ct + _NESTING_TAB_OFFSET,
                _default_multiline,
                force_singleline == FORCE_SINGLELINE ?
                false : _default_multiline );
        }
    };
    /**
     * @brief Whitespace format for parentheses nesting depth 0.
     * @ingroup non_data_text_formatting
     */
    _Whitespace _ROOT_WS { 0, settings.multiline };
    /**
     * @brief Determines field width in characters for fmt library printing of
     *   "{:#0x}"
     * @tparam ScalarT scalar type of val
     * @param val scalar to format
     * @ingroup individual_data_field_formatting
     */
    template< typename ScalarT,
              std::enable_if_t< std::is_scalar_v< ScalarT >, bool > = true >
    inline constexpr size_t _fmtHexWidth( const ScalarT val ) {
        // fmt counts "0x" as part of width when using '#'
        return ( sizeof( val ) * 2 ) + ( sizeof( "0x" ) - 1 );
    }
    /**
     * @brief Byteswaps an integral value depending on params.
     * @tparam IntegralT integral type of val, up to 4 bytes
     * @param val integral to byteswap
     * @param byteswap whether to byteswap
     * @note [_LESSTHANOREQUALTO](#_LESSTHANOREQUALTO) macro used to circumvent
     *   Doxygen (v1.9.8) parsing bug.
     * @ingroup individual_data_field_formatting
     */
    template< typename IntT,
              std::enable_if_t< std::is_integral_v< IntT > &&
                                sizeof( IntT ) _LESSTHANOREQUALTO sizeof( uint32_t ),
                  bool > = true >
    inline IntT _hostByteOrder( const IntT val, const bool byteswap ) {
        switch ( sizeof( IntT ) ) {
        case sizeof( uint32_t ):
            return byteswap ? __builtin_bswap32( val ) : val;
        case sizeof( uint16_t ):
            return byteswap ? __builtin_bswap16( val ) : val;
        default:
            break;
        }
        return val;  // sizeof( uint8_t )
    }
    /**
     * @brief Describes range for which a value has enum names.
     * @ingroup individual_data_field_formatting
     */
    class _EnumNameRange {
    public:
        /**
         * @brief Differentiates lower and upper bounds when using single-bound
         *   ctor.
         */
        enum class Bound { MIN, MAX };

    private:
        /**
         * @brief Sentinel to indicate uninitialized bounds.
         */
        static constexpr uint32_t _UNINIT {
            std::numeric_limits< uint32_t >::max() };
        /**
         * @brief Sentinel to indicate empty name array.
         */
        inline static const std::vector< std::string_view > _NO_NAMES {};

    public:
        /**
         * @brief Enum names by value.
         */
        const std::vector< std::string_view >& names { _NO_NAMES };
        /**
         * @brief Minimum enum value used.
         */
        uint32_t min { _UNINIT };
        /**
         * @brief Maximum enum value used.
         */
        uint32_t max { _UNINIT };

        _EnumNameRange() {}
        /**
         * @brief Ctor to define both lower and upper bound of enum values.
         */
        _EnumNameRange( const std::vector< std::string_view >& names_,
                        const uint32_t min_, const uint32_t max_ ) :
            names( names_ ), min( min_ ), max( max_ ) {
            assert( !names.empty() );
            assert( max < names_.size() );
            assert( min <= max );
        }
        /**
         * @brief Ctor to define either lower and upper bound of enum values.
         */
        _EnumNameRange( const std::vector< std::string_view >& names_,
                        const Bound bound, const uint32_t value ) :
            _EnumNameRange( names_,
                            bound == Bound::MIN ? value : 0,
                            bound == Bound::MIN ? names_.size() - 1 : value ) {}
        /**
         * @brief Ctor implying all enum values in names array used.
         */
        _EnumNameRange( const std::vector< std::string_view >& names_ ) :
            _EnumNameRange( names_, 0, names_.size() - 1 ) {}
        /**
         * @brief Determines if a value is in the range.
         * @tparam IntT integral type of val
         * @param val integral to test
         * @return whether value is in range
         */
        template< typename IntT,
                  std::enable_if_t< std::is_integral_v< IntT >, bool > = true >
        inline bool in( const IntT val ) const {
            if ( std::is_signed_v< IntT > && val < 0 )
                return false;
            if ( names.empty() )
                return false;
            return ( std::make_unsigned_t< IntT >( val ) >= min &&
                     std::make_unsigned_t< IntT >( val ) <= max );
        }
        /**
         * @brief Finds enum name at value.
         * @tparam IntT integral type of val
         * @param val integral to find in range
         * @return enum name at value, or empty string if out of range
         */
        template< typename IntT,
                  std::enable_if_t< std::is_integral_v< IntT >, bool > = true >
        inline std::string_view at( const IntT val ) const {
            return in( val ) ? names[ val ] : "";
        }
        /**
         * @brief Determines if range is empty.
         * @return whether range is empty
         */
        inline bool empty() const {
            return names.empty();
        }
    };
    /**
     * @brief Bundles enum name range and bitmask status to be passed into
     *   higher-level parsing functions.
     * @ingroup individual_data_field_formatting
     */
    struct _ValueTraits {
        /**
         * @brief Range of values with named enum.
         */
        _EnumNameRange name_range {};
        /**
         * @brief Whether value should be parsed as a bitmask.
         */
        bool           bitmask    { false };
        /**
         * @brief Syntax-sweetening constants for ctor param `bitmask`.
         */
        enum { NOT_BITMASK, BITMASK };
        _ValueTraits() {}
        /**
         * @brief Expected ctor.
         * @param name_range_ range of values with named enum
         * @param bitmask_ whether value should be parsed as a bitmask
         */
        _ValueTraits( const _EnumNameRange name_range_,
                      const bool bitmask_ = NOT_BITMASK ) :
            name_range( name_range_ ), bitmask( bitmask_ ) {}
    };
    /**
     * @brief Format plain integers or protocol integer aliases (eg CARD8) based
     *   on settings and params.
     * @tparam IntT integral type of val
     * @param encoded_val integral to format
     * @param byteswap whether to byteswap before formatting
     * @param name_range range of values for which `encoded_val` has enum name
     * @param bitmask whether `encoded_val` is a bitmask
     * @return formatted string representing `encoded_val`
     * @ingroup individual_data_field_formatting
     */
    template< typename IntT,
              std::enable_if_t< std::is_integral_v< IntT >, bool > = true >
    std::string _formatVariable( const IntT encoded_val, const bool byteswap,
                                 const _EnumNameRange name_range = {},
                                 const bool bitmask = _ValueTraits::NOT_BITMASK ) {
        const std::string hex_str {
            fmt::format( "{:#0{}x}", std::make_unsigned_t< IntT >( encoded_val ),
                         _fmtHexWidth( encoded_val ) ) };
        const IntT val { _hostByteOrder( encoded_val, byteswap ) };

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
    /**
     * @brief Format non-ATOM X ID/resource ID value based on settings and params.
     * @tparam ResourceIdT non-ATOM protocol resource ID type (eg WINDOW)
     * @param var variable to format
     * @param byteswap whether to byteswap before formatting
     * @param name_range range of values for which `var` has enum names
     * @return formatted string representing `var`
     * @ingroup individual_data_field_formatting
     */
    template< typename ResourceIdT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::ResourceId, ResourceIdT > &&
                  !std::is_same_v< protocol::ATOM, ResourceIdT >,
                  bool > = true >
    std::string _formatVariable( const ResourceIdT var,
                                 const bool byteswap,
                                 const _EnumNameRange name_range = {} ) {
        assert( ( var.data & ResourceIdT::ZERO_BITS ) == 0 );
        return _formatVariable( var.data, byteswap, name_range );
    }
    /**
     * @brief Format protocol type DRAWABLE based on settings and params.
     * @param var DRAWABLE to format
     * @param byteswap whether to byteswap before formatting
     * @param name_range range of values for which `var` has enum names
     * @return formatted string representing `var`
     * @note DRAWABLE not appropriate for `ResourceIdT` template due to it being
     *   implmented as a union of WINDOW and PIXMAP.
     * @ingroup individual_data_field_formatting
     */
    std::string _formatVariable( const protocol::DRAWABLE var,
                                 const bool byteswap,
                                 const _EnumNameRange name_range = {} );
    /**
     * @brief Format protocol type FONTABLE based on settings and params.
     * @param var FONTABLE to format
     * @param byteswap whether to byteswap before formatting
     * @param name_range range of values for which `var` has enum names
     * @return formatted string representing `var`
     * @note FONTABLE not appropriate for `ResourceIdT` template due to it being
     *   implmented as a union of FONT and GCONTEXT.
     * @ingroup individual_data_field_formatting
     */
    std::string _formatVariable( const protocol::FONTABLE var,
                                 const bool byteswap,
                                 const _EnumNameRange name_range = {} );
    /**
     * @brief Format protocol resource ID type ATOM based on settings and params.
     * @param var ATOM to format
     * @param byteswap whether to byteswap before formatting
     * @param name_range range of values for which `var` has enum names
     * @return formatted string representing `var`
     * @note ATOM not appropriate for `ResourceIdT` template due to special
     *   handling required for internment and stashing (see
     *   [_internStashedAtom](#_internStashedAtom).)
     * @ingroup individual_data_field_formatting
     */
    std::string _formatVariable( const protocol::ATOM var,
                                 const bool byteswap,
                                 const _EnumNameRange name_range = {} );
    /**
     * @brief Format non-resource ID protocol integer type (eg TIMESTAMP) based
     *   on settings and params.
     * @tparam ProtocolIntT non-resource ID protocol integer type
     * @param var to format
     * @param byteswap whether to byteswap before formatting
     * @param name_range range of values for which `var` has enum names
     * @return formatted string representing `var`
     * @ingroup individual_data_field_formatting
     */
    template< typename ProtocolIntT,
              std::enable_if_t<
                  ( std::is_base_of_v< protocol::impl::Integer, ProtocolIntT > &&
                    !std::is_base_of_v< protocol::impl::ResourceId, ProtocolIntT > ),
                  bool > = true >
    std::string _formatVariable( const ProtocolIntT var,
                                 const bool byteswap,
                                 const _EnumNameRange name_range = {} );
    /**
     * @brief Format simple (no variable length suffix) protocol struct types
     *   (eg ARC) based on settings and params.
     * @tparam ProtocolStructT simple protocol struct type
     * @param var to format
     * @param byteswap whether to byteswap before formatting
     * @param ws whitespace formatting based on settings and indentation
     * @return formatted string representing `var`
     * @ingroup individual_data_field_formatting
     */
    template< typename ProtocolStructT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::Struct, ProtocolStructT > &&
                  !std::is_base_of_v<
                      protocol::impl::StructWithSuffixes, ProtocolStructT >,
                  bool > = true >
    std::string _formatVariable( const ProtocolStructT var,
                                 const bool byteswap,
                                 const _Whitespace& ws );
    /**
     * @brief Bundles formatted string output with count of bytes parsed into
     *   single return type for higher-level parsing functions.
     * @ingroup LISTofT_formatting
     */
    struct _ParsingOutputs {
        /**
         * @brief Formatted string.
         */
        std::string str       {};
        /**
         * @brief Total data bytes parsed.
         */
        uint32_t bytes_parsed {};
    };
    /**
     * @brief Parse plain integer or protocol integer alias (eg CARD8) from raw
     *   bytes, based on settings and params.
     * @tparam ProtocolT plain integer or protocol integer alias (eg CARD8)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param byteswap whether to byteswap before formatting
     * @param traits enum name range and bitmask status of `var`
     * @return formatted string and count of bytes parsed
     * @ingroup LISTofT_formatting
     */
    template< typename ProtocolT,
              std::enable_if_t< std::is_integral_v< ProtocolT >,
                                bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz, const bool byteswap,
                      const _ValueTraits traits = {} ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        const ProtocolT memb { *reinterpret_cast< const ProtocolT* >( data ) };
        return { _formatVariable( memb, byteswap,
                                  traits.name_range, traits.bitmask ),
                 sizeof( ProtocolT ) };
    }
    // protocol integer types (eg ResourceId, Bitmask)
    /**
     * @brief Parse protocol integer type (not alias) (eg WINDOW TIMESTAMP
     *   SETofEVENT) from raw bytes, based on settings and params.
     * @tparam ProtocolT protocol integer types (not aliases) (eg WINDOW
     *   TIMESTAMP SETofEVENT)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param byteswap whether to byteswap before formatting
     * @param traits enum name range and bitmask status of `var`
     * @return formatted string and count of bytes parsed
     * @ingroup LISTofT_formatting
     */
    template< typename ProtocolT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::Integer, ProtocolT >,
                  bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz, const bool byteswap,
                      const _ValueTraits traits = {} ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolT ) );

        const ProtocolT memb { *reinterpret_cast< const ProtocolT* >( data ) };
        return { _formatVariable( memb, byteswap, traits.name_range ),
                 sizeof( ProtocolT ) };
    }
    /**
     * @brief Parse simple (no variable length suffix) protocol struct type
     *   (eg ARC) from raw bytes, based on settings and params.
     * @tparam ProtocolStructT simple protocol struct integer type (eg ARC)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param byteswap whether to byteswap before formatting
     * @param ws whitespace formatting based on settings and indentation
     * @return formatted string and count of bytes parsed
     * @ingroup LISTofT_formatting
     */
    template< typename ProtocolStructT,
              std::enable_if_t<
                  std::is_base_of_v< protocol::impl::Struct, ProtocolStructT > &&
                  !std::is_base_of_v<
                      protocol::impl::StructWithSuffixes, ProtocolStructT >,
                  bool > = true >
    _ParsingOutputs
    _parseListMember( const uint8_t* data, const size_t sz, const bool byteswap,
                      const _Whitespace& ws ) {
        assert( data != nullptr );
        assert( sz >= sizeof( ProtocolStructT ) );

        const ProtocolStructT memb {
            *reinterpret_cast< const ProtocolStructT* >( data ) };
        return { _formatVariable( memb, byteswap, ws ),
                 sizeof( ProtocolStructT ) };
    }
    /**
     * @brief Parse protocol struct type with variable length suffix (eg STR)
     *   or union containing such a struct (eg TEXTITEM8) from raw bytes, based
     *   on settings and params.
     * @tparam ProtocolStructT variable length protocol struct type (eg STR),
     *   TEXTITEM8|16
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param byteswap whether to byteswap before formatting
     * @param ws whitespace formatting based on settings and indentation
     * @return formatted string and count of bytes parsed
     * @ingroup LISTofT_formatting
     */
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
    _parseListMember( const uint8_t* data, const size_t sz, const bool byteswap,
                      const _Whitespace& ws );
    /**
     * @brief Parse LISTof* from raw bytes, based on settings and params, when
     *   no count of list members is provided (eg PolyText8|16).
     * @tparam ProtocolT protocol type of list members
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param byteswap whether to byteswap before formatting
     * @param ws whitespace formatting based on settings and indentation
     * @param force_membs_singleline whether to format list members as singleline,
     *   independent of LIST whitespace formatting
     * @return formatted string and count of bytes parsed
     * @note [_PARSELISTMEMBER](#_PARSELISTMEMBER) macro used to circumvent
     *   Doxygen (v1.9.8) parsing bug.
     * @ingroup LISTofT_formatting
     */
    template< typename ProtocolT,
              std::enable_if_t<
                  ( !std::is_integral_v< ProtocolT > &&
                    !std::is_base_of_v< protocol::impl::Integer, ProtocolT > ),
                  bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz,
                  const bool byteswap, const _Whitespace& ws,
                  const bool force_membs_singleline = _Whitespace::DEFAULT ) {
        assert( data != nullptr );

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    sz == 0 ? "" : ws.separator );
        while ( alignment.pad( outputs.bytes_parsed ) < sz ) {
            const _ParsingOutputs member {
                _PARSELISTMEMBER( ProtocolT )(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    byteswap, ws.nested( force_membs_singleline ) ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", outputs.bytes_parsed == 0 ? "" : ws.encl_indent );
        return outputs;
    }
    /**
     * @brief Parse LISTof* from raw bytes, based on settings and params, when
     *   T is a protocol non-integer type (eg ARC STR).
     * @tparam ProtocolT protocol type of list members
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param memb_ct number of items in list
     * @param byteswap whether to byteswap before formatting
     * @param ws whitespace formatting based on settings and indentation
     * @param force_membs_singleline whether to format list members as singleline,
     *   independent of LIST whitespace formatting
     * @return formatted string and count of bytes parsed
     * @note [_PARSELISTMEMBER](#_PARSELISTMEMBER) macro used to circumvent
     *   Doxygen (v1.9.8) parsing bug.
     * @ingroup LISTofT_formatting
     */
    template< typename ProtocolT,
              std::enable_if_t<
                  ( !std::is_integral_v< ProtocolT > &&
                    !std::is_base_of_v< protocol::impl::Integer, ProtocolT > ),
                  bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t memb_ct,
                  const bool byteswap, const _Whitespace& ws,
                  const bool force_membs_singleline = _Whitespace::DEFAULT ) {
        assert( data != nullptr );

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    memb_ct == 0 ? "" : ws.separator );
        for ( uint16_t i {}; i < memb_ct; ++i ) {
            const _ParsingOutputs member {
                _PARSELISTMEMBER( ProtocolT )(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    byteswap, ws.nested( force_membs_singleline ) ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", memb_ct == 0 ? "" : ws.encl_indent );
        return outputs;
    }
    /**
     * @brief Parse LISTof* from raw bytes, based on settings and params, when
     *   T is a plain integer, protocol integer alias (eg CARD8), or protocol
     *   integer type (eg WINDOW).
     * @tparam IntegerT integer type of list members
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param memb_ct number of items in list
     * @param byteswap whether to byteswap before formatting
     * @param ws whitespace formatting based on settings and indentation
     * @param traits enum name range and bitmask status of list member type
     * @return formatted string and count of bytes parsed
     * @note [_PARSELISTMEMBER](#_PARSELISTMEMBER) macro used to circumvent
     *   Doxygen (v1.9.8) parsing bug.
     * @ingroup LISTofT_formatting
     */
    template< typename IntegerT,
              std::enable_if_t<
                  std::is_integral_v< IntegerT > ||
                  std::is_base_of_v< protocol::impl::Integer, IntegerT >,
                  bool > = true >
    _ParsingOutputs
    _parseLISTof( const uint8_t* data, const size_t sz, const uint16_t memb_ct,
                  const bool byteswap, const _Whitespace& ws,
                  const _ValueTraits traits = {} ) {
        assert( data != nullptr );

        _ParsingOutputs outputs;
        outputs.str += fmt::format( "[{}",
                                    memb_ct == 0 ? "" : ws.separator );
        for ( uint16_t i {}; i < memb_ct; ++i ) {
            const _ParsingOutputs member {
                _PARSELISTMEMBER( IntegerT )(
                    data + outputs.bytes_parsed, sz - outputs.bytes_parsed,
                    byteswap, traits ) };
            outputs.bytes_parsed += member.bytes_parsed;
            outputs.str += fmt::format(
                "{}{}{}", ws.memb_indent, member.str, ws.separator );
        }
        outputs.str += fmt::format(
            "{}]", memb_ct == 0 ? "" : ws.encl_indent );
        return outputs;
    }
    /**
     * @brief Bundles all inputs needed by [_parseLISTofVALUE](#_parseLISTofVALUE).
     * @tparam TupleT type of tuple containing all list member types
     * @ingroup LISTofVALUE_formatting
     */
    template< typename TupleT >
    struct _LISTofVALUEParsingInputs {
        /**
         * @brief Bytes to parse.
         */
        const uint8_t* data;
        /**
         * @brief Maximum bytes available to parse.
         */
        const size_t sz;
        /**
         * @brief Bitmask where a 1 at a given bit index indicates that the
         *   VALUE of the corresponding index in [types](#types),
         *   [names](#names), and [traits](#traits) appears in the list.
         */
        const uint32_t value_mask;
        /**
         * @brief Heterogeneous list of LISTofVALUE member types.
         */
        const TupleT& types;
        /**
         * @brief List of LISTofVALUE member names.
         */
        const std::vector< std::string_view >& names;
        /**
         * @brief List of LISTofVALUE member value traits for use in parsing.
         */
        const std::vector< _ValueTraits >& traits;
        /**
         * @brief Whether VALUEs must be byte-swapped before logging.
         */
        const bool byteswap;
        /**
         * @brief Whitespace formatting for list.
         */
        const _Whitespace ws;
        /**
         * @brief Field width in chars of longest list member name.
         */
        size_t memb_name_w {};

        _LISTofVALUEParsingInputs() = delete;
        /**
         * @brief Default constructor.
         * @param data_ bytes to parse
         * @param sz_ maximum bytes available to parse
         * @param value_mask_ bitmask indicating list membership
         * @param types_ heterogeneous list of LISTofVALUE member types
         * @param names_ list of LISTofVALUE member names
         * @param traits_ list of LISTofVALUE member value traits
         * @param byteswap_ whether VALUEs must be byte-swapped before logging
         * @param ws_ whitespace formatting for list
         */
        _LISTofVALUEParsingInputs(
            const uint8_t* data_, const size_t sz_, const uint32_t value_mask_,
            const TupleT& types_, const std::vector< std::string_view >& names_,
            const std::vector< _ValueTraits >& traits_,
            const bool byteswap_ , const _Whitespace ws_ ) :
            data( data_ ), sz( sz_ ), value_mask( value_mask_ ),
            types( types_ ), names( names_ ), traits( traits_ ),
            byteswap( byteswap_ ), ws( ws_ ) {

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
    /**
     * @brief Bundles all outputs of [_parseLISTofVALUE](#_parseLISTofVALUE).
     * @ingroup LISTofVALUE_formatting
     */
    struct _LISTofVALUEParsingOutputs : public _ParsingOutputs {
        /**
         * @brief Count of VALUEs in LISTofVALUE.
         */
        uint32_t value_ct {};
    };
    /**
     * @brief Parse LISTofVALUE from raw bytes, based on settings and params.
     * @tparam I index of current tuple member
     * @param inputs see [_LISTofVALUEParsingInputs](#_LISTofVALUEParsingInputs)
     * @param[out] outputs see [_LISTofVALUEParsingOutputs](#_LISTofVALUEParsingOutputs)
     * @note Employs tuple iteration pattern using recursive templating, allowing
     *   for runtime traversal of tuple members. This overload is the recursive
     *   base case, ending recursion once index I has reached the tuple size.
     * @ingroup LISTofVALUE_formatting
     */
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
    /**
     * @brief Parse LISTofVALUE from raw bytes, based on settings and params.
     * @tparam I index of current tuple member
     * @param inputs see [_LISTofVALUEParsingInputs](#_LISTofVALUEParsingInputs)
     * @param[out] outputs see [_LISTofVALUEParsingOutputs](#_LISTofVALUEParsingOutputs)
     * @note Employs tuple iteration pattern using recursive templating, allowing
     *   for runtime traversal of tuple members.
     * @note [_LESSTHAN](#_LESSTHAN) macro used to circumvent Doxygen (v1.9.8)
     *   parsing bug.
     * @ingroup LISTofVALUE_formatting
     */
    template< size_t I = 0, typename... Args,
              std::enable_if_t< I _LESSTHAN sizeof...( Args ), bool > = true >
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
            _PARSELISTMEMBER( ValueT )(
                inputs.data + outputs->bytes_parsed,
                inputs.sz - outputs->bytes_parsed, inputs.byteswap,
                inputs.traits[ I ] ) };
        // do not use member.bytes_parsed as all VALUEs are encoded as 4B
        outputs->bytes_parsed += sizeof( protocol::VALUE );

        outputs->str += fmt::format(
            "{}{: <{}}{}{}{}",
            inputs.ws.memb_indent, inputs.names[ I ], inputs.memb_name_w,
            inputs.ws.equals, member.str, inputs.ws.separator );
        return _parseLISTofVALUE< I + 1, Args... >( inputs, outputs );
    }
    /**
     * @brief Parse server-bound client packet as X11 message, and print it to
     *   log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    size_t _logClientPacket(
            Connection* conn, uint8_t* data, const size_t sz );
    /**
     * @brief Parse client-bound server packet as X11 message, and print it to
     *   log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    size_t _logServerPacket(
        Connection* conn, uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 connection setup message, and print it to log file stream.
     * @tparam ConnectionSetupT type of connection setup message encoding (eg
     *   protocol::connection_setup::Acceptance)
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    template< typename ConnectionSetupT >
    size_t _logConnectionSetup(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 request from raw bytes.
     * @tparam RequestT type of request encoding (eg protocol::requests::InternAtom)
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return formatted string and bytes parsed
     * @ingroup logging
     */
    template< typename RequestT >
    _ParsingOutputs _parseRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 request, and print it to log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    size_t _logRequest(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 reply from raw bytes.
     * @tparam RequestT type of request encoding (eg protocol::requests::InternAtom)
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return formatted string and bytes parsed
     * @ingroup logging
     */
    template< typename RequestT >
    _ParsingOutputs _parseReply(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 reply, and print it to log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    size_t _logReply(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 event from raw bytes.
     * @tparam EventT type of event encoding (eg protocol::events::KeyPress)
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param indents whitespace formatting to use
     * @return formatted string and bytes parsed
     * @ingroup logging
     */
    template< typename EventT >
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Whitespace& indents );
    /**
     * @brief Parse X11 event from raw bytes.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @param indents whitespace formatting to use
     * @return formatted string and bytes parsed
     * @note This function is a wrapper around its templated equivalent, used to
     *   accommodate parsing of both normal event encoding and events passed as
     *   data members of a SendEvent request.
     * @ingroup logging
     */
    _ParsingOutputs _parseEvent(
        Connection* conn, const uint8_t* data, const size_t sz,
        const _Whitespace& indents );
    /**
     * @brief Parse X11 event, and print it to log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    size_t _logEvent(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 error from raw bytes.
     * @tparam ErrorT type of error encoding (eg protocol::errors::Value)
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return formatted string and bytes parsed
     * @ingroup logging
     */
    template< typename ErrorT >
    _ParsingOutputs _parseError(
        Connection* conn, const uint8_t* data, const size_t sz );
    /**
     * @brief Parse X11 error, and print it to log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @param data bytes to parse
     * @param sz maximum bytes readable from `data`
     * @return bytes parsed
     * @ingroup logging
     */
    size_t _logError(
        Connection* conn, const uint8_t* data, const size_t sz );

public:
    /**
     * @brief Parse server-bound client packets as X11 messages, and print them to
     *   log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @return bytes parsed
     * @ingroup logging
     */
    size_t logClientPackets( Connection* conn );
    /**
     * @brief Parse client-bound server packets as X11 messages, and print them to
     *   log file stream.
     * @param[in,out] conn status of current connection, see [Connection](#Connection)
     * @return bytes parsed
     * @ingroup logging
     */
    size_t logServerPackets( Connection* conn );
    /**
     * @brief Encoding memory alignment calculator.
     */
    _Alignment alignment;
    /**
     * @brief User settings imported from [ProxyX11Server](#ProxyX11Server).
     */
    Settings settings;
    /**
     * @brief Import settings from [ProxyX11Server](#ProxyX11Server) during startup.
     * @param settings_ reference to server settings
     * @param fetched_atoms reference atom strings fetched from actual X server,
     *   representing contiguous interned ATOMs from 1 to n
     */
    void importSettings( const Settings& settings_,
                         const std::vector< std::string >& fetched_atoms );
};

#undef _LESSTHAN
#undef _LESSTHANOREQUALTO
#undef _PARSELISTMEMBER


#endif  // X11PROTOCOLPARSER_HPP
