#ifndef DISPLAYINFO_HPP
#define DISPLAYINFO_HPP


#include <string>
#include <string_view>


// TBD how does this compare to libX11 Display (_XDisplay)?
//   - https://github.com/mirror/libX11/blob/master/include/X11/Xlibint.h#L72
class DisplayInfo {
private:
    static constexpr std::string_view _UNIX_SOCKET_PATH_PREFIX {
        "/tmp/.X11-unix/X" };
    static constexpr int _UNSET { -1 };

public:
    std::string unparsed_name;
    std::string name;
    std::string protocol;
    std::string hostname;
    int display { _UNSET };
    int screen  { _UNSET };
    int family  { _UNSET };
    std::string unix_socket_path;

    //_DisplayInfo() = delete;
    DisplayInfo() {}
    DisplayInfo(const char* displayname);
};


#endif  // DISPLAYINFO_HPP
