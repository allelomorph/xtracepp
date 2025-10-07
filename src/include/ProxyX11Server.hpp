#ifndef PROXYX11SERVER_HPP
#define PROXYX11SERVER_HPP


#include <atomic>
#include <string>
#include <string_view>
#include <optional>
#include <unordered_map>
#include <set>
#include <map>
#include <vector>

#include <cstdint>      // SIZE_MAX

#include <poll.h>       // struct pollfd
#include <signal.h>     // siginfo_t

#include "DisplayInfo.hpp"
#include "Settings.hpp"
#include "Connection.hpp"
#include "X11ProtocolParser.hpp"


// TBD using lock-free std::atomic for signal handler per C++ standard
extern std::atomic_bool child_running;
static_assert( decltype( child_running )::is_always_lock_free );
extern std::atomic_int  child_retval;
static_assert( decltype( child_retval )::is_always_lock_free );

void handleSIGCHLD( int sig, siginfo_t* info, void* ucontext );

extern std::atomic<const char*> in_display_sun_path;
static_assert( decltype( in_display_sun_path )::is_always_lock_free );
extern std::atomic<const char*> out_display_sun_path;
static_assert( decltype( out_display_sun_path )::is_always_lock_free );
extern std::atomic<const char*> xauth_path;
static_assert( decltype( xauth_path )::is_always_lock_free );
extern std::atomic<const char*> xauth_bup_path;
static_assert( decltype( xauth_bup_path )::is_always_lock_free );

void handleTerminatingSignal( int sig );

class ProxyX11Server {
public:
    Settings settings;

    ProxyX11Server() {}
    ~ProxyX11Server();

    // TBD `char* const* argv` required by getopt_long in Settings
    void init( const int argc, char* const* argv );
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
    std::string _xauth_path;
    std::string _xauth_bup_path;
    struct _XAuthInfo;
    void _copyAuthentication();

    ////// Pre-queue clients to probe for setup info

    bool _authenticateServerConnection(
        const int server_fd, protocol::WINDOW* screen0_root = nullptr );
    void _fetchCurrentServerTime();
    void _fetchInternedAtoms();

    ////// CLI Subcommand

    bool  _child_used { false };
    static constexpr pid_t _UNINIT_PID { -1 };
    pid_t _child_pid  { _UNINIT_PID };

    void _startSubcommandClient();

    ////// Socket polling

    static constexpr short _POLLNONE {};
    // TBD std::map over std::unordered_map to allow iteration
    std::map< int, size_t > _pfds_i_by_fd;
    std::vector< pollfd >   _pfds;

    void _addSocketToPoll( const int fd, const short events = _POLLNONE );
    void _updatePollFlags();
    inline bool _socketReadReady( const int fd ) {
        return ( _pfds.at( _pfds_i_by_fd.at( fd ) ).revents & POLLIN );
    }
    inline bool _socketWriteReady( const int fd ) {
        return ( _pfds.at( _pfds_i_by_fd.at( fd ) ).revents & POLLOUT );
    }
    std::optional< std::string_view >
    _socketPollError( const int fd );
    void _processPolledSockets();

    ////// Main client queue

    static constexpr int _MAX_PENDING_CONNECTIONS { 20 };

    static constexpr int _UNINIT_FD { -1 };
    int _listener_fd  { _UNINIT_FD };
    std::unordered_map< int, Connection > _connections;

    void _listenForClients();
    bool _acceptClient( Connection* conn );
    int  _connectToServer();
    void _openConnection();
    void _closeConnections( const std::vector< int >& ids );
    int  _processClientQueue();

    X11ProtocolParser _parser;
};


#endif  // PROXYX11SERVER_HPP
