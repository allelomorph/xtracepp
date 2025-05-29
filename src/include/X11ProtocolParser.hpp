#ifndef X11PROTOCOLPARSER_HPP
#define X11PROTOCOLPARSER_HPP


//#include <ostream>
//#include <iostream>  // cout
#include <tuple>
#include <string>
#include <string_view>
#include <type_traits>  // enable_if_t remove_reference_t
#include <vector>
#include <algorithm>  // max

#include <cassert>

#include <stdio.h>  // stdout

#include <fmt/format.h>

#include "Connection.hpp"
#include "Settings.hpp"


class X11ProtocolParser {
private:
    static constexpr size_t _ALIGN { 4 };

    // TBD make member of server instead?
    FILE* _log_fs { stdout };

    void _bufferHexDump( const uint8_t* data, const size_t sz );

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

    // TBD pass in verbosity flag?
    template < typename T >
    auto _formatCommonType(
        const T value,
        const std::vector< std::string_view >& enum_names = {},
        const size_t max_enum = 0, const size_t min_enum = 0 ) ->
        std::enable_if_t<std::is_integral_v<T>, std::string> {

        std::string result { fmt::format( "{:d}", value ) };
        if ( max_enum == 0 && !enum_names.empty() )
            max_enum = enum_names.size() - 1;
        for ( size_t enum_ { min_enum }; enum_ <= max_enum; ++enum_ ) {
            if ( enum_ == value ) {
                result += fmt::format( " ({})", enum_names[enum_] );
                break;
            }
        }
        return result;
    }

    // std::string _formatCommonType( const TIMESTAMP  time );
    // std::string _formatCommonType( const CURSOR     cursor );
    // std::string _formatCommonType( const COLORMAP   colormap );
    // std::string _formatCommonType( const ATOM       atom );
    // std::string _formatCommonType( const VISUALID   visualid );
    // std::string _formatCommonType( const WINDOW     window );
    // std::string _formatCommonType( const PIXMAP     pixmap );
    // std::string _formatCommonType( const DRAWABLE   drawable );
    // std::string _formatCommonType( const FONT       font );
    // std::string _formatCommonType( const GCONTEXT   gcontext );
    // std::string _formatCommonType( const FONTABLE   fontable );
    // std::string _formatCommonType( const BITGRAVITY bitgravity );
    // std::string _formatCommonType( const WINGRAVITY wingravity );
    // std::string _formatCommonType( const BOOL       bool_ );
    // std::string _formatCommonType( const SETofEVENT setofevent );
    // std::string _formatCommonType( const SETofPOINTEREVENT setofpointerevent );
    // std::string _formatCommonType( const SETofDEVICEEVENT setofdeviceevent );
    // std::string _formatCommonType( const KEYSYM     keysym );
    // std::string _formatCommonType( const KEYCODE    keycode );
    // std::string _formatCommonType( const BUTTON     button );
    // std::string _formatCommonType( const SETofKEYMASK setofkeymask );
    // std::string _formatCommonType( const SETofKEYBUTMASK setofkeybutmask );
    // // TBD STR parsed differently
    // // TBD CHAR2B parsed differently
    // std::string _formatCommonType( const POINT      point );
    // std::string _formatCommonType( const RECTANGLE  rectangle );
    // std::string _formatCommonType( const ARC        arc );
    // std::string _formatCommonType( const HOST       host );

    struct _LISTofVALUEParsingOutputs {
        std::string str     {};
        int values_parsed   {};
        size_t bytes_parsed {};
    };

    struct _LISTofVALUEParsingInputs {
        struct _VALUEParsingStrings {
            std::string_view name;
            std::string_view fmt_specifier;
            const std::vector<std::string_view>& enum_names;
        };
        const std::vector< _VALUEParsingStrings >& strings;
        size_t name_width {};
        std::string indent;

        _LISTofVALUEParsingInputs() = delete;
        _LISTofVALUEParsingInputs(
            const std::vector< _VALUEParsingStrings >& _strings,
            const std::string _indent ) :
            strings( _strings ), indent( _indent ) {
            for ( const _VALUEParsingStrings& strs : _strings ) {
                name_width = std::max( name_width, strs.name.size() );
            }
            ++name_width;
        }
    };

    // tuple iteration allows for run time taversal of heterogeneous list of types
    // TBD all this may not be necessary - could we just parse all as uint32_t and cast as necessary?
    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args >
    auto _parseLISTofVALUE(
        const uint32_t /*value_mask*/, const std::tuple< Args... >& /*value_types*/,
        const _LISTofVALUEParsingInputs& /*inputs*/,
        const uint8_t* /*data*/, _LISTofVALUEParsingOutputs* /*outputs*/ ) ->
        std::enable_if_t< I == sizeof...( Args ), void > {
    }

    // TBD recommended tuple iteration pattern using recursive templating
    template< size_t I = 0, typename... Args >
    auto _parseLISTofVALUE(
        const uint32_t value_mask, const std::tuple< Args... >& value_types,
        const _LISTofVALUEParsingInputs& inputs,
        const uint8_t* data, _LISTofVALUEParsingOutputs* outputs ) ->
        std::enable_if_t< I < sizeof...( Args ), void > {

        // TBD not efficient to have static vars in templated function...
        static constexpr int ALIGN { 4 };
        const _LISTofVALUEParsingInputs::_VALUEParsingStrings& strings {
            inputs.strings[ I ] };
        if ( value_mask & ( 1 << I ) ) {
            using ValueT = std::remove_reference_t<
                decltype( std::get< I >( value_types ) ) >;
            ValueT value { *reinterpret_cast< ValueT* >( data + ( I * ALIGN ) ) };
            std::string format {
                std::string( "{}{: <{}}: {:" ) + strings.fmt_specifier.data() + "}{}\n" };
            outputs->str += fmt::format(
                format, inputs.indent, strings.name, inputs.name_width, value,
                ( !strings.enum_names.empty() &&
                  value < int( strings.enum_names.size() ) ) ?
                fmt::format( " ({})", strings.enum_names[ value ] ) : "" );
            outputs->values_parsed  += 1;
            outputs->bytes_parsed   += ALIGN;
        }
        _parseLISTofVALUE< I + 1, Args... >(
            value_mask, value_types, inputs, data, outputs );
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

    void setLogFileStream( FILE* log_fs );
    size_t logClientPackets( Connection* conn, Settings* settings );
    size_t logServerPackets( Connection* conn, Settings* settings );
};


#endif  // X11PROTOCOLPARSER_HPP
