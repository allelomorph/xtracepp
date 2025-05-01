#ifndef PROXYX11SERVER_HPP
#define PROXYX11SERVER_HPP


#include <string>
#include <string_view>
#include <optional>
#include <unordered_map>

#include <cstdint>      // SIZE_MAX

#include "DisplayInfo.hpp"
#include "Settings.hpp"
#include "Connection.hpp"


extern volatile bool caught_SIGCHLD;

void catchSIGCHLD(int signum);

class ProxyX11Server {
private:
    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "PROXYDISPLAY" };
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };

    static constexpr int _X_TCP_PORT { 6000 };
    static constexpr int _MAX_PENDING_CONNECTIONS { 20 };

    DisplayInfo _in_display;   // used to set traits of listener socket
    DisplayInfo _out_display;  // used to set traits of sockets connecting to x server

    int _listener_fd { -1 };  // listening for x clients to intercept comms with x server

    pid_t _child_pid { -1 };  // cli subcmd pid

    std::unordered_map<int, Connection> connections;

    void _parseDisplayNames();

    void _listenForClients();
    void _startSubcommandClient();
    bool _acceptClient(Connection* conn);
    int _connectToServer();
    std::optional<Connection> _acceptConnection();
    int _prepareSocketFlagging( fd_set* readfds, fd_set* writefds,
                                fd_set* exceptfds );
    void _processFlaggedSockets( fd_set* readfds, fd_set* writefds,
                                 fd_set* exceptfds );
    int _processClientQueue();

    void __debugOutput();

public:
    Settings settings;

    ProxyX11Server();
    ~ProxyX11Server();

    void init(const int argc, char* const* argv);
    int run();
};


#endif  // PROXYX11SERVER_HPP
