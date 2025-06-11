#ifndef X11PROTOCOLPARSER_HPP
#define X11PROTOCOLPARSER_HPP


//#include <ostream>
//#include <iostream>  // cout
#include <tuple>  // tuple_sz
#include <string>
#include <string_view>
#include <type_traits>  // enable_if_t remove_reference_t
#include <vector>
#include <algorithm>  // max
#include <limits>

#include <cassert>

#include <stdio.h>  // stdout

#include <fmt/format.h>

#include "Connection.hpp"
#include "Settings.hpp"
#include "protocol/enum_names.hpp"
#include "protocol/common_types.hpp"


class X11ProtocolParser {
private:
    // TBD change to PAD_ALIGN_SZ to differentiate from VALUE default size
    static constexpr size_t _ALIGN { 4 };

    // TBD make member of server instead?
    FILE* _log_fs { stdout };

    Settings::Verbosity _verbosity {};

    void _bufferHexDump( const uint8_t* data, const size_t sz );

    static constexpr std::string_view _WHITESPACE {
        "                                                           "
        "                                                           "
    };
    static constexpr uint32_t _TAB_SZ { 2 };  // in spaces
    inline std::string_view _makeIndent( const uint32_t tab_ct ) {
        assert( tab_ct * _TAB_SZ <= _WHITESPACE.size() );
        return { _WHITESPACE.data(), tab_ct * _TAB_SZ };
    }

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
        return ( value & _TOP_3_OF_32_BITS == 0 );
    }

    // TBD sentinel necessary?
    static constexpr uint32_t _UNINITIALIZED {
        std::numeric_limits< uint32_t >::max() };

    // TBD [u]intXX_t CARDXX INTXX
    template < typename ValueT >
    auto _formatInteger(
        const ValueT value,
        const std::vector< std::string_view >& enum_names = {},
        uint32_t max_enum = _UNINITIALIZED,
        uint32_t min_enum = _UNINITIALIZED ) ->
        std::enable_if_t< std::is_integral_v< ValueT >, std::string > {

        std::string name_str {};
        if ( !enum_names.empty() ) {
            assert( value != _UNINITIALIZED );
            if ( max_enum == _UNINITIALIZED )
                max_enum = enum_names.size() - 1;
            if ( min_enum == _UNINITIALIZED )
                min_enum = 0;
            if ( value <= ValueT( max_enum ) && value >= ValueT( min_enum ) )
                name_str = std::string( enum_names[ value ] );
        }
        if ( _verbosity == Settings::Verbosity::Debug ) {
            // fmt counts "0x" as part of width when using '#'
            static constexpr size_t hex_width { ( sizeof( value ) * 2 ) + 2 };
            const std::string hex_str { fmt::format( "{:#0{}x}", value, hex_width ) };
            return name_str.empty() ? hex_str :
                fmt::format( "{} ({})", hex_str, name_str );
        }
        return name_str.empty() ? fmt::format( "{:d}", value ) : name_str;
    }

    template < typename MaskT >
    auto _formatBitmask(
        const MaskT mask,
        const std::vector< std::string_view >& flag_names = {},
        uint32_t max_flag_i = _UNINITIALIZED ) ->
        std::enable_if_t< std::is_integral_v< MaskT > && !std::is_signed_v< MaskT >,
                          std::string > {
        // fmt counts "0x" as part of width when using '#'
        static constexpr size_t hex_width { ( sizeof( mask ) * 2 ) + 2 };
        std::string hex_str { fmt::format( "{:#0{}x}", mask, hex_width ) };
        std::string flag_str;
        if ( !flag_names.empty() ) {
            if ( max_flag_i == _UNINITIALIZED )
                max_flag_i = flag_names.size() - 1;
            for ( uint32_t i {}; i <= max_flag_i; ++i ) {
                if ( mask & ( 1 << i ) ) {
                    flag_str.append( flag_str.empty() ? "" : " & " );
                    flag_str.append( flag_names[i] );
                }
            }
        }
        if ( _verbosity == Settings::Verbosity::Debug ) {
            return flag_str.empty() ? hex_str :
                fmt::format( "{} ({})", hex_str, flag_str );
        }
        return flag_str.empty() ? hex_str : flag_str;
    }

    std::string
    _formatCommonType( const protocol::TIMESTAMP time );

    inline std::string
    _formatCommonType( const protocol::CURSOR cursor ) {
        return _formatInteger( cursor.data,
                               protocol::enum_names::zero_none );
    }

    // COLORMAP could use zero_none or zero_copy_from_parent
    std::string
    _formatCommonType( const protocol::COLORMAP colormap,
                       const std::vector< std::string_view >& enum_names = {} );

    std::string
    _formatCommonType( const protocol::ATOM atom );

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

    // TBD DRAWABLE?

    inline std::string
    _formatCommonType( const protocol::FONT font ) {
        return _formatInteger( font.data,
                               protocol::enum_names::zero_none );
    }

    // TBD GCONTEXT?
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

    inline std::string
    _formatCommonType( const protocol::SETofEVENT setofevent ) {
        // SETofEVENT
        //     #xFE000000     unused but must be zero
        return _formatBitmask( setofevent.data,
                               protocol::enum_names::set_of_event );
    }

    std::string
    _formatCommonType( const protocol::SETofPOINTEREVENT setofpointerevent );
    std::string
    _formatCommonType( const protocol::SETofDEVICEEVENT setofdeviceevent );

    inline std::string
    _formatCommonType( const protocol::KEYCODE keycode ) {
        return _formatInteger( keycode.data,
                               protocol::enum_names::key );
    }

    inline std::string
    _formatCommonType( const protocol::BUTTON button ) {
        return _formatInteger( button.data,
                               protocol::enum_names::button );
    }

    std::string
    _formatCommonType( const protocol::SETofKEYMASK setofkeymask );

    inline std::string
    _formatCommonType( const protocol::SETofKEYBUTMASK setofkeybutmask ) {
        // SETofKEYBUTMASK
        //   #xE000     unused but must be zero
        return _formatBitmask( setofkeybutmask.data,
                               protocol::enum_names::set_of_keybutmask );
    }

    std::string
    _formatCommonType( const protocol::POINT point );
    std::string
    _formatCommonType( const protocol::RECTANGLE rectangle );
    std::string
    _formatCommonType( const protocol::ARC arc );

    // TBD HOST?

    struct _LISTofVALUEParsingOutputs {
        std::string str     {};
        int values_parsed   {};
    };

    struct _EnumTraits {
        const std::vector< std::string_view >& names;
        const uint32_t min;
        const uint32_t max;
        const bool is_mask {};

        _EnumTraits(
            const std::vector<std::string_view>& names_ = {},
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
        const std::string_view& indent {};
        size_t name_width {};

        _LISTofVALUEParsingInputs() = delete;
        _LISTofVALUEParsingInputs(
            const TupleT& types_, const std::vector<std::string_view>& names_,
            const std::vector< _EnumTraits >& traits_,
            const std::string_view& indent_ ) :
            types( types_ ), names( names_ ),
            traits( traits_ ), indent( indent_ ) {
            assert( std::tuple_size< TupleT >{} == names.size() &&
                    names.size() == traits.size() );
            assert( indent.size() >= _TAB_SZ );
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
        const uint32_t /*value_mask*/,
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        const uint8_t* /*data*/, _LISTofVALUEParsingOutputs* outputs ) ->
        std::enable_if_t< I == sizeof...( Args ), void > {
        if ( _verbosity == Settings::Verbosity::Singleline ) {
            outputs->str += " ]";
        } else {
            outputs->str += fmt::format(
                "{}]", _makeIndent( ( inputs.indent.size() / _TAB_SZ ) - 1 ) );
        }
    }

    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args >
    auto _parseLISTofVALUE(
        const uint32_t value_mask,
        const _LISTofVALUEParsingInputs< std::tuple< Args... > >& inputs,
        const uint8_t* data, _LISTofVALUEParsingOutputs* outputs ) ->
        std::enable_if_t< I < sizeof...( Args ), void > {
        if ( outputs->str.empty() ) {
            outputs->str += '[';
            outputs->str +=
                _verbosity == Settings::Verbosity::Singleline ? ' ' : '\n';
        }
        using ValueT = std::remove_reference_t<
            decltype( std::get< I >( inputs.types ) ) >;
        if ( value_mask & ( 1 << I ) ) {
            const ValueT value { *reinterpret_cast< const ValueT* >( data ) };
            const std::string value_str { _formatVALUE( value, inputs.traits[I] ) };
            if ( _verbosity == Settings::Verbosity::Singleline ) {
                outputs->str += fmt::format( "{}{}={}",
                                             outputs->values_parsed != 0 ? ", " : "",
                                             inputs.names[I], value_str );
            } else {
                outputs->str += fmt::format( "{}{: <{}} = {}\n",
                                             inputs.indent,
                                             inputs.names[I],
                                             inputs.name_width,
                                             value_str );
            }
            outputs->values_parsed += 1;
        }
        _parseLISTofVALUE< I + 1, Args... >(
            value_mask, inputs, data + _VALUE_ENCODING_SZ, outputs );
    }

    size_t _logConnectionInitiation(
        const Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logServerResponse(
        Connection* conn, const uint8_t* data, const size_t sz );

    size_t _logClientPacket(
        Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerPacket(
        Connection* conn, uint8_t* data, const size_t sz );

    size_t _logServerError(
        Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerReply(
        Connection* conn, uint8_t* data, const size_t sz );
    size_t _logServerEvent(
        Connection* conn, uint8_t* data, const size_t sz,
        const uint8_t code );

    size_t _logClientRequest(
        Connection* conn, const uint8_t* data, const size_t sz,
        const uint8_t opcode );
    size_t _logCreateWindow(
        Connection* conn, const uint8_t* data, const size_t sz );
    size_t _logChangeWindowAttributes(
        Connection* conn, const uint8_t* data, const size_t sz );
    // size_t _logGetWindowAttributes(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logDestroyWindow(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logDestroySubwindows(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeSaveSet(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logReparentWindow(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logMapWindow(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logMapSubwindows(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUnmapWindow(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUnmapSubwindows(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logConfigureWindow(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCirculateWindow(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetGeometry(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryTree(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logInternAtom(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetAtomName(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeProperty(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logDeleteProperty(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetProperty(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logListProperties(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetSelectionOwner(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetSelectionOwner(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logConvertSelection(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSendEvent(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGrabPointer(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUngrabPointer(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGrabButton(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUngrabButton(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeActivePointerGrab(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGrabKeyboard(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUngrabKeyboard(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGrabKey(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUngrabKey(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logAllowEvents(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGrabServer(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUngrabServer(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryPointer(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetMotionEvents(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logTranslateCoordinates(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logWarpPointer(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetInputFocus(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetInputFocus(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryKeymap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logOpenFont(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCloseFont(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryFont(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryTextExtents(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logListFonts(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logListFontsWithInfo(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetFontPath(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetFontPath(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCreatePixmap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logFreePixmap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCreateGC(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeGC(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCopyGC(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetDashes(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetClipRectangles(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logFreeGC(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logClearArea(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCopyArea(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCopyPlane(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyPoint(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyLine(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolySegment(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyRectangle(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyArc(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logFillPoly(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyFillRectangle(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyFillArc(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPutImage(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetImage(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyText8(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logPolyText16(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logImageText8(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logImageText16(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCreateColormap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logFreeColormap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCopyColormapAndFree(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logInstallColormap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logUninstallColormap(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logListInstalledColormaps(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logAllocColor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logAllocNamedColor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logAllocColorCells(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logAllocColorPlanes(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logFreeColors(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logStoreColors(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logStoreNamedColor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryColors(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logLookupColor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCreateCursor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logCreateGlyphCursor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logFreeCursor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logRecolorCursor(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryBestSize(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logQueryExtension(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logListExtensions(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeKeyboardMapping(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetKeyboardMapping(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeKeyboardControl(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetKeyboardControl(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logBell(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangePointerControl(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetPointerControl(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetScreenSaver(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetScreenSaver(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logChangeHosts(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logListHosts(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetAccessControl(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetCloseDownMode(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logKillClient(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logRotateProperties(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logForceScreenSaver(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetPointerMapping(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetPointerMapping(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logSetModifierMapping(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logGetModifierMapping(
    //     Connection* conn, uint8_t* data, const size_t sz );
    // size_t _logNoOperation(
    //     Connection* conn, uint8_t* data, const size_t sz );

public:
    X11ProtocolParser() {}

    // TBD maybe import FILE*, verbosity, and readwritedebug all in one
    //   importSettings func call
    void setLogFileStream( FILE* log_fs );
    inline void setVerbosity( const Settings::Verbosity verbosity ) {
        _verbosity = verbosity;
    }
    size_t logClientPackets( Connection* conn, const Settings& settings );
    size_t logServerPackets( Connection* conn, const Settings& settings );
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
