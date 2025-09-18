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
    static constexpr std::string_view _UNIX_SOCKET_PATH_PREFIX {
        "/tmp/.X11-unix/X" };
    static constexpr int _UNSET { -1 };

    static constexpr std::string_view _UNIX  { "unix" };
    static constexpr std::string_view _TCP   { "tcp" };
    static constexpr std::string_view _LOCAL { "local" };
    static constexpr std::string_view _INET  { "inet" };

    // TBD matches UNIX_PATH_MAX from <linux/un.h>
    static constexpr size_t _UNIX_PATH_MAX { 108 };
    static constexpr int _X_TCP_PORT { 6000 };

    char _ipv4_addrstr_buf[ INET_ADDRSTRLEN ] {};

    // TBD regex matching groups
    enum {
        _PROTOCOL_GROUP_I = 1,
        _HOSTNAME_GROUP_I,
        _COLON_GROUP_I,
        _DISPLAY_GROUP_I,
        _SCREEN_GROUP_I
    };
public:
    std::string name;
    std::string protocol;
    std::string hostname;
    int display { _UNSET };
    int screen  { _UNSET };
    int family  { _UNSET };
    union {
        sockaddr    addr;  // TBD inclusion in union saves us a cast when calling bind(2) or connect(2)
        sockaddr_in inaddr;
        sockaddr_un unaddr {};
        static_assert( sizeof( unaddr.sun_path ) == _UNIX_PATH_MAX );
    };
    std::string_view ipv4_addr { _ipv4_addrstr_buf, INET_ADDRSTRLEN };

    enum class Direction {
        IN, OUT };
    //_DisplayInfo() = delete;
    DisplayInfo() {}
    //DisplayInfo( const char* displayname );
    DisplayInfo( const char* displayname, const Direction direction );
};


#endif  // DISPLAYINFO_HPP
