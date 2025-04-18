#ifndef INTERMEDIARYSERVER_HPP
#define INTERMEDIARYSERVER_HPP


#include <string>
#include <string_view>

#include <cstdint>      // SIZE_MAX

#include "Settings.hpp"

class IntermediaryServer {
private:
    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "FAKEDISPLAY" };
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };

    static constexpr int _X_TCP_PORT { 6000 };

    class _DisplayInfo {
    private:
        static constexpr std::string_view _UNIX_SOCKET_PATH_PREFIX {
            "/tmp/.X11-unix/X" };
        static constexpr int _UNSET { -1 };

    public:
        std::string name;
        std::string protocol;
        std::string hostname;
        int display { _UNSET };
        int screen  { _UNSET };
        int family  { _UNSET };
        std::string unix_socket_path;

        //_DisplayInfo() = delete;
        _DisplayInfo() {}
        _DisplayInfo(const char* displayname);
    };

    _DisplayInfo _in_display;
    _DisplayInfo _out_display;

    int _in_fd;    // listening for x clients to intercept comms with x server
    int _out_fd;   // comms with x server on behalf of x clients

    pid_t _child_pid;  // cli subcmd pid

public:
    Settings settings;

    IntermediaryServer();
    ~IntermediaryServer();

    void parseDisplayNames();
    void listenForClients();
    void startClient();

    void __debugOutput();
};


#endif  // INTERMEDIARYSERVER_HPP
