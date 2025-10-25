#ifndef DISPLAYINFO_HPP
#define DISPLAYINFO_HPP


#include <string>
#include <string_view>

#include <netinet/in.h>  // sockaddr_in INET_ADDRSTRLEN
// TBD <linux/un.h> provides alternative sockaddr_un with UNIX_PATH_MAX
#include <sys/un.h>      // sockaddr_un


// TBD how does this compare to libX11 Display (_XDisplay)?
//   - https://github.com/mirror/libX11/blob/master/include/X11/Xlibint.h#L72
class DisplayInfo {
private:
    static constexpr int _UNSET { -1 };

    static constexpr std::string_view _UNIX  { "unix" };
    static constexpr std::string_view _TCP   { "tcp" };
    static constexpr std::string_view _LOCAL { "local" };
    static constexpr std::string_view _INET  { "inet" };
    static constexpr std::string_view _INET6 { "inet6" };

    // should match UNIX_PATH_MAX from <linux/un.h>
    static constexpr size_t _UNIX_PATH_MAX           { 108 };
    static constexpr int    _SOCKET_DEFAULT_PROTOCOL { 0 };
    // https://x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
    // "For TCP connections, displays on a given host are numbered starting from
    //   0, and the server for display N listens and accepts connections on port
    //   6000 + N."
    static constexpr int    _X_TCP_PORT              { 6000 };
    static constexpr std::string_view
                            _UNIX_SOCKET_PATH_PREFIX { "/tmp/.X11-unix/X" };

    char _addrstr_buf[ INET6_ADDRSTRLEN ] {};
    bool _unix_pattern {};

    // TBD regex matching groups
    enum {
        _UNIX_PROTOCOL_GROUP_I = 1,
        _UNIX_SOCKET_PATH_GROUP_I,
        _UNIX_SCREEN_GROUP_I
    };
    enum {
        _PROTOCOL_GROUP_I = 1,
        _HOSTNAME_GROUP_I,
        _DISPLAY_GROUP_I,
        _SCREEN_GROUP_I
    };
public:
    std::string name;
    std::string protocol;
    std::string hostname;
    std::string socket_path;
    int display { _UNSET };
    int screen  { _UNSET };
    int ai_family   { _UNSET };
    int ai_socktype { _UNSET };
    int ai_protocol { _UNSET };
    socklen_t ai_addrlen {};
    union {
        // including sockaddr in union saves a cast when calling bind(2) or connect(2)
        sockaddr     ai_addr;
        sockaddr_in  inaddr;
        sockaddr_in6 in6addr;
        sockaddr_un  unaddr {};
        static_assert( sizeof( unaddr ) > sizeof( ai_addr ) &&
                       sizeof( unaddr ) > sizeof( inaddr ) &&
                       sizeof( unaddr ) > sizeof( in6addr ) );
        static_assert( sizeof( unaddr.sun_path ) == _UNIX_PATH_MAX );
    };
    std::string_view addrstr { _addrstr_buf, INET6_ADDRSTRLEN };

    enum class Direction {
        IN, OUT };
    DisplayInfo() {}
    DisplayInfo( const char* displayname, const Direction direction );
};


#endif  // DISPLAYINFO_HPP
