#ifndef PROXYX11SERVER_HPP
#define PROXYX11SERVER_HPP


#include <string>
#include <string_view>

#include <cstdint>      // SIZE_MAX

#include "DisplayInfo.hpp"
#include "Settings.hpp"
#include "Connection.hpp"


class ProxyX11Server {
private:
    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "FAKEDISPLAY" };
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };

    static constexpr int _X_TCP_PORT { 6000 };

    DisplayInfo _in_display;   // used to set traits of listener socket
    DisplayInfo _out_display;  // used to set traits of sockets connecting to x server

    int _listener_fd;  // listening for x clients to intercept comms with x server
//    int _out_fd;       // comms with x server on behalf of x clients

    pid_t _child_pid { -1 };  // cli subcmd pid

public:
    Settings settings;

    ProxyX11Server();
    ~ProxyX11Server();

    void parseDisplayNames();
    void listenForClients();
    void startClient();
    bool acceptClient(Connection* conn);
    int connectToServer();
    Connection acceptConnection();
    int processClientQueue();

    void __debugOutput();
};


#endif  // PROXYX11SERVER_HPP
