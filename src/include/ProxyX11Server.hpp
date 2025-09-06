#ifndef PROXYX11SERVER_HPP
#define PROXYX11SERVER_HPP


#include <string>
#include <string_view>
#include <unordered_map>
#include <set>

#include <cstdint>      // SIZE_MAX

#include "DisplayInfo.hpp"
#include "Settings.hpp"
#include "Connection.hpp"
#include "X11ProtocolParser.hpp"


extern volatile bool caught_SIGCHLD;

void catchSIGCHLD(int signum);

class ProxyX11Server {
private:
    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "PROXYDISPLAY" };
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };

    static constexpr int _X_TCP_PORT { 6000 };
    static constexpr int _MAX_PENDING_CONNECTIONS { 20 };

    // TBD only supporting MIT-MAGIC-COOKIE-1 for now
    static constexpr std::string_view _AUTH_NAME { "MIT-MAGIC-COOKIE-1" };
    static constexpr uint16_t         _AUTH_DATA_SZ { 16 };
    // TBD for later use in atom prefetching
    uint8_t auth_data[ _AUTH_DATA_SZ ];

    DisplayInfo _in_display;   // used to set traits of listener socket
    DisplayInfo _out_display;  // used to set traits of sockets connecting to x server

    int _listener_fd { -1 };  // listening for x clients to intercept comms with x server
    pid_t _child_pid { -1 };  // cli subcmd pid

    std::unordered_map<int, Connection> _connections;
    // maximum binary tree of open file descriptors, to supply nfds to select(2)
    std::set<int, std::greater<int>>    _open_fds;

    // TBD these are identical with X11ProtocolParser, and are needed for
    //   _authenticateServerConnection, _fetchCurrentServerTime, and
    //   _fetchInternedAtoms- can we be friends with X11ProtocolParser without circular include?
    // "where E is some expression, and pad(E) is the number of bytes needed to
    //   round E up to a multiple of four."
    static constexpr size_t _ALIGN { 4 };
    inline size_t _padToAlignment( const size_t n, const size_t align ) {
        return n + ( ( align - ( n % align ) ) % align );
    };
    inline size_t _pad( const size_t n ) {
        return _padToAlignment( n, _ALIGN );
    }
    inline size_t _alignedUnits( const size_t sz ) {
        return sz / _ALIGN;
    }

    X11ProtocolParser parser;

    void _parseDisplayNames();

    struct XAuthInfo;
    void _copyAuthentication();

    void _listenForClients();
    void _startSubcommandClient();
    bool _acceptClient(Connection* conn);
    int  _connectToServer();
    void _acceptConnection();
    int  _prepareSocketFlagging( fd_set* readfds, fd_set* writefds,
                                fd_set* exceptfds );
    void _processFlaggedSockets( fd_set* readfds, fd_set* writefds,
                                 fd_set* exceptfds );
    int  _processClientQueue();

    void __debugOutput();

    void _pollSingleSocket(
        const int socket_fd, const short events, int timeout = -1 );
    bool _authenticateServerConnection(
        const int server_fd, protocol::WINDOW* screen0_root = nullptr );
    void _fetchCurrentServerTime();
    void _fetchInternedAtoms();

public:
    Settings settings;

    ProxyX11Server();
    ~ProxyX11Server();

    void init(const int argc, char* const* argv);
    int run();
};


#endif  // PROXYX11SERVER_HPP
