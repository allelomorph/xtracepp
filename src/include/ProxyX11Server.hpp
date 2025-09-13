#ifndef PROXYX11SERVER_HPP
#define PROXYX11SERVER_HPP


#include <string>
#include <string_view>
#include <unordered_map>
#include <set>
#include <functional>   // greater
#include <map>
#include <vector>

#include <cstdint>      // SIZE_MAX

#include <poll.h>       // struct pollfd

#include "DisplayInfo.hpp"
#include "Settings.hpp"
#include "Connection.hpp"
#include "X11ProtocolParser.hpp"


extern volatile bool caught_SIGCHLD;

void catchSIGCHLD(int signum);

class ProxyX11Server {
public:
    Settings settings;

    ProxyX11Server() {}
    ~ProxyX11Server();

    // TBD `char* const* argv` required by getopt_long in Settings
    void init(const int argc, char* const* argv);
    int run();

private:
    ////// Parsing display names

    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "PROXYDISPLAY" };
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };

    DisplayInfo _in_display;   // used to set traits of listener socket
    DisplayInfo _out_display;  // used to set traits of sockets connecting to x server

    void _parseDisplayNames();

    ////// Ensuring auth for _out_display matches _in_display

    // TBD only supporting MIT-MAGIC-COOKIE-1 for now
    static constexpr std::string_view _AUTH_NAME { "MIT-MAGIC-COOKIE-1" };
    static constexpr uint16_t         _AUTH_DATA_SZ { 16 };
    uint8_t _auth_data[ _AUTH_DATA_SZ ];

    struct _XAuthInfo;
    void _copyAuthentication();

    ////// Pre-queue clients to probe for setup info

    void _pollSingleSocket(
        const int socket_fd, const short events, int timeout = -1 );
    bool _authenticateServerConnection(
        const int server_fd, protocol::WINDOW* screen0_root = nullptr );
    void _fetchCurrentServerTime();
    void _fetchInternedAtoms();

    ////// Main client queue

    static constexpr int _X_TCP_PORT { 6000 };
    static constexpr int _MAX_PENDING_CONNECTIONS { 20 };

    int _listener_fd { -1 };  // listening for x clients to intercept comms with x server
    pid_t _child_pid { -1 };  // cli subcmd pid

    std::unordered_map<int, Connection> _connections;
    std::set<int, std::greater<int>>    _open_fds;

    // std::map to allow iteration
    std::map< int, size_t > _pfds_i_by_fd;
    std::vector< pollfd >   _pfds;

    void _listenForClients();
    void _startSubcommandClient();
    bool _acceptClient( Connection* conn );
    int  _connectToServer();
    void _acceptConnection();
    int  _prepareSocketFlagging( fd_set* readfds, fd_set* writefds,
                                 fd_set* exceptfds );
    void _processFlaggedSockets( fd_set* readfds, fd_set* writefds,
                                 fd_set* exceptfds );
    int  _processClientQueue();

    X11ProtocolParser _parser;
};


#endif  // PROXYX11SERVER_HPP
