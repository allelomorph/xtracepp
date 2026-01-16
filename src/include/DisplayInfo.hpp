#ifndef DISPLAYINFO_HPP
#define DISPLAYINFO_HPP

/**
 * @file DisplayInfo.hpp
 */

#include <string>
#include <string_view>
#include <regex>

#include <netinet/in.h>  // sockaddr_in sockaddr_in6 INET_ADDRSTRLEN
// linux/un.h provides a more straightforward definition of sockaddr_un
//   along with UNIX_PATH_MAX, but sys/un.h seems more portable as it is the
//   header referenced in sockaddr(3type)
#include <sys/un.h>      // sockaddr_un


/**
 * @brief Encapsulates X display name parsing.
 * @note `ai_` prefixed members are named after corresponding `addrinfo`
 *    (`getaddrinfo(3)`) members
 */
class DisplayInfo {
private:
    /**
     * @brief Sentinel indicating uninitialized enum value.
     */
    static constexpr int _UNSET { -1 };
    /**
     * @brief Possible [protocol](#DisplayInfo::protocol) value.
     */
    static constexpr std::string_view _UNIX  { "unix" };
    /**
     * @brief Possible [protocol](#DisplayInfo::protocol) value.
     */
    static constexpr std::string_view _TCP   { "tcp" };
    /**
     * @brief Possible [protocol](#DisplayInfo::protocol) value.
     */
    static constexpr std::string_view _LOCAL { "local" };
    /**
     * @brief Possible [protocol](#DisplayInfo::protocol) value.
     */
    static constexpr std::string_view _INET  { "inet" };
    /**
     * @brief Possible [protocol](#DisplayInfo::protocol) value.
     */
    static constexpr std::string_view _INET6 { "inet6" };
    /**
     * @brief Copy of UNIX_PATH_MAX from <linux/un.h> to avoid header dependence.
     */
    static constexpr size_t _UNIX_PATH_MAX           { 108 };
    /**
     * @brief Syntax sweetening constant for `protocol` parameter of `socket(2)`.
     */
    static constexpr int    _SOCKET_DEFAULT_PROTOCOL { 0 };
    /**
     * @brief Base value for X server TCP [port numbers].
     * [port numbers]: https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
     */
    static constexpr int    _X_TCP_PORT              { 6000 };
    /**
     * @brief Base filepath for unix sockets.
     */
    static constexpr std::string_view
                            _UNIX_SOCKET_PATH_PREFIX { "/tmp/.X11-unix/X" };
    /**
     * @brief Unix socket-based display name parsing regex.
     * @note libX11 and libxcb both parse display names with call chains
     *   ending in [_xcb_parse_display], which contains this [undocumented]
     *   pattern for unix socket-based display names:  
     *     [`unix:`]socket_path[`.`screen_number]
     *
     *   [_xcb_parse_display]: https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/e81b999a/src/xcb_util.c#L160
     *   [undocumented]: https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Opening_the_Display
     */
    inline static const std::regex _UNIX_REGEX {
        "^(?:(unix):)?"      // [protocol:]
        "(.+?)"              // socket_path
        "(?:\\.([0-9]+))?$"  // [.screen_number]
    };
    /**
     * @brief Regex group match indices (from 1) of [unix socket pattern](#_UNIX_REGEX).
     */
    enum {
        _UNIX_PROTOCOL_GROUP_I = 1,
        _UNIX_SOCKET_PATH_GROUP_I,
        _UNIX_SCREEN_GROUP_I
    };
    /**
     * @brief Default hostname or URI-based display name parsing regex.
     * @note libX11 and libxcb both parse display names with call chains
     *   ending in [_xcb_parse_display], which contains this [documented]
     *   TCP pattern (minus DECnet) for hostname or URI-based display names
     *   (defaulting to a unix sockets if no hostname is provided):  
     *     [[protocol`/`]hostname\|URI]`:`display_number[`.`screen_number]
     *
     *   [_xcb_parse_display]: https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/e81b999a/src/xcb_util.c#L160
     *   [documented]: https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Opening_the_Display
     */
    inline static const std::regex _DEFAULT_REGEX {
        "^(?:"
            "(?:([a-zA-Z6]+)/)?"  // [ [<protocol>/]
            "([^\\s]+?)"          // <hostname/URI> ]
        ")?"
        "(?="
            ":([0-9]+)"           // :<display number>
            "(?:\\.([0-9]+))?"    // [.<screen number>]
        "$)"
    };
    /**
     * @brief Regex group match indices (from 1) of [default pattern](#_DEFAULT_REGEX).
     */
    enum {
        _PROTOCOL_GROUP_I = 1,
        _HOSTNAME_GROUP_I,
        _DISPLAY_GROUP_I,
        _SCREEN_GROUP_I
    };
    /**
     * @brief `char` buffer for use with `inet_ntop(3)`.
     */
    char _addrstr_buf[ INET6_ADDRSTRLEN ] {};
    /**
     * @brief Whether display name string matches the unix socket pattern.
     */
    bool _unix_pattern {};

public:
    /**
     * @brief Unmodified display name string.
     */
    std::string name;
    /**
     * @brief Protocol name token extracted from [name](#name), used to help set
     *   [ai_family](#ai_family).
     * @note [Ignored] by libX11/libxcb.
     * [Ignored]: https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/e81b999a/src/xcb_util.c#L238
     */
    std::string protocol;
    /**
     * @brief Hostname token extracted from [name](#name).
     */
    std::string hostname;
    /**
     * @brief Unix socket path; populated if [ai_family](#ai_family) is `AF_UNIX`.
     */
    std::string socket_path;
    /**
     * @brief X display index.
     */
    int display { _UNSET };
    /**
     * @brief X screen index.
     */
    int screen  { _UNSET };
    /**
     * @brief `AF_*` macro for `domain` parameter to `socket(2)`.
     */
    int ai_family   { _UNSET };
    /**
     * @brief `SOCK_*` macro for `type` param to `socket(2)`.
     */
    int ai_socktype { _UNSET };
    /**
     * @brief Macro for `protocol` param to `socket(2)`.
     */
    int ai_protocol { _UNSET };
    /**
     * @brief Length in bytes of appropriate `sockaddr(3type)` type.
     */
    ::socklen_t ai_addrlen {};
    /**
     * @brief Superimposition of all possible `sockaddr(3type)` types.
     * @note Including generic `sockaddr` (named after `addrinfo`
     *   (`getaddrinfo(3)`) member) in union saves a cast when calling
     *   `bind(2)` or `connect(2)`.
     */
    union {
        ::sockaddr     ai_addr;
        ::sockaddr_in  inaddr;
        ::sockaddr_in6 in6addr;
        ::sockaddr_un  unaddr {};
    };
    static_assert( sizeof( unaddr ) > sizeof( ai_addr ) &&
                   sizeof( unaddr ) > sizeof( inaddr ) &&
                   sizeof( unaddr ) > sizeof( in6addr ) );
    static_assert( sizeof( unaddr.sun_path ) == _UNIX_PATH_MAX );
    /**
     * @brief View into address buffer populated by `inet_ntop(3)`.
     */
    std::string_view addrstr { _addrstr_buf, INET6_ADDRSTRLEN };
    /**
     * @brief Differentiates IN display (logging proxy server) from OUT
     *   display (actual X server).
     */
    enum class Direction { IN, OUT };

    DisplayInfo() {}
    /**
     * @brief Parses X display name into tokens used in logging messages and
     *   data needed for socket creation.
     * @param displayname unparsed X display name string
     * @param direction toggle indicating whether proxy server or real server,
     *   and thus whether socket will be used by `listen(2)` or `connect(2)`
     * @param process_name name of app executable for error messages
     */
    DisplayInfo( const char* displayname, const Direction direction,
                 const std::string_view& process_name );
};


#endif  // DISPLAYINFO_HPP
