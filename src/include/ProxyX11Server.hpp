#ifndef PROXYX11SERVER_HPP
#define PROXYX11SERVER_HPP

/**
 * @file ProxyX11Server.hpp
 */

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


/**
 * @brief Represents a proxy server that intercepts, logs, and sometimes
 *   modifies messages sent between an X server and its clients.
 */
class ProxyX11Server {
public:
    ProxyX11Server() = delete;
    /**
     * @brief Default ctor; initializes #Settings from CLI options.
     * @param argc count of CLI tokens
     * @param argv array of CLI tokens
     */
    ProxyX11Server( const int argc, const char* argv[] );
    ~ProxyX11Server();
    /**
     * @brief Options selected by user in CLI options and relevant environmental
     *   variables.
     */
    Settings settings;
    /**
     * @brief Added to signal values to disambiguate signal termination from
     *   other exit codes.
     */
    static constexpr int SIGNAL_RETVAL_OFFSET { 128 };
    /**
     * @brief Begin operation.
     * @return `EXIT_SUCCESS` by default, `EXIT_FAILURE` on error. If using
     *   subcommand child process but not `--keeprunning`, child return value or
     *   signal value + [SIGNAL_RETVAL_OFFSET](#SIGNAL_RETVAL_OFFSET) if child
     *   stopped by signal.
     */
    int run();

private:
    /**
     * @brief Name of environmental variable containing the "in" (pointing to
     *   this proxy server) X display name.
     * @ingroup parsing_display_names
     */
    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "PROXYDISPLAY" };
    /**
     * @brief Name of environmental variable containing the "out" (pointing to
     *   real X server) X display name.
     * @ingroup parsing_display_names
     */
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    /**
     * @brief Default, and most minimal, string to use for the "in" X display
     *   name.
     * @ingroup parsing_display_names
     */
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };
    /**
     * @brief All logging and socket operation data derived from the "in"
     *   (pointing to this proxy server) X display name.
     * @ingroup parsing_display_names
     */
    DisplayInfo _in_display;
    /**
     * @brief All logging and socket operation data derived from the "out"
     *   (pointing to real X server) X display name.
     * @ingroup parsing_display_names
     */
    DisplayInfo _out_display;
    /**
     * @brief Gets X display names from environmental variables, and uses them
     *   to populate [_in_display](#_in_display) and [_out_display](#_out_display).
     * @ingroup parsing_display_names
     */
    void _parseDisplayNames();
    /**
     * @brief Name of only supported [X authentication protocol].
     * [X authentication protocol]: https://www.x.org/releases/current/doc/man/man7/Xsecurity.7.xhtml
     * @ingroup copy_authentication
     */
    static constexpr std::string_view _AUTH_NAME { "MIT-MAGIC-COOKIE-1" };
    /**
     * @brief Size of [MIT-MAGIC-COOKIE-1] data in bytes.
     * [MIT-MAGIC-COOKIE-1]: https://www.x.org/releases/current/doc/man/man7/Xsecurity.7.xhtml
     * @ingroup copy_authentication
     */
    static constexpr uint16_t         _AUTH_DATA_SZ { 16 };
    /**
     * @brief [MIT-MAGIC-COOKIE-1] data buffer.
     * [MIT-MAGIC-COOKIE-1]: https://www.x.org/releases/current/doc/man/man7/Xsecurity.7.xhtml
     * @ingroup copy_authentication
     */
    uint8_t _auth_data[ _AUTH_DATA_SZ ];
    /**
     * @brief Full filepath to original `xauth(1)` authorization file.
     * @ingroup copy_authentication
     */
    std::string _xauth_path;
    /**
     * @brief Full filepath to backup of `xauth(1)` authorization file made
     *   while modifying original's contents.
     * @ingroup copy_authentication
     */
    std::string _xauth_bup_path;
    struct _XAuthInfo;
    /**
     * @brief Modifies `xauth(1)` authorization file to ensure that an entry
     *   for [_in_display](#_in_display) exists and duplicates the authorization
     *   data for [_out_display](#_out_display).
     * @ingroup copy_authentication
     */
    void _copyAuthentication();
    /**
     * @brief Performs handshake between pre-queue setup client and X server.
     * @param[in]  server_fd file descriptor of X server
     * @param[out] screen0_root if not default `nullptr`,
     *   [WINDOW](#protocol::WINDOW) pointed to set to `root` field of
     *   first [SCREEN](#protocol::connection_setup::Acceptance::SCREEN) in
     *   LISTofSCREEN `roots` at end of server's [acceptance] of initial handshake
     * @return `true` if successful, `false` if not
     * [acceptance]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Encoding::Connection_Setup
     * @ingroup pre_queue_setup_clients
     */
    bool _authenticateServerConnection(
        const int server_fd, protocol::WINDOW* screen0_root = nullptr );
    /**
     * @brief Optional setup client run before main queue begins; sets
     *   [ref_TIMESTAMP](#Settings::ref_TIMESTAMP) and
     *   [ref_unix_time](#Settings::ref_unix_time) in
     *   [settings](#settings) to support `--systemtimeformat` user option.
     * @ingroup pre_queue_setup_clients
     */
    void _fetchCurrentServerTime();
    /**
     * @brief Optional setup client run before main queue begins; duplicates
     *   real X server's string interments for contiguous range of
     *   [ATOM](#protocol::ATOM)s 1 to n to support `--prefetchatoms` user option.
     * @return vector of interned atom strings, indexed by
     *   [ATOM](#protocol::ATOM) value
     * @ingroup pre_queue_setup_clients
     */
    std::vector< std::string > _fetchInternedAtoms();
    /**
     * @brief Whether CLI subcommand was provided and launched as
     *   child process.
     * @ingroup cli_subcommand
     */
    bool  _child_used { false };
    /**
     * @brief Sentinel value indicating uninitialized process id.
     * @ingroup cli_subcommand
     */
    static constexpr pid_t _UNINIT_PID { -1 };
    /**
     * @brief Process id of CLI subcommand client child process.
     * @ingroup cli_subcommand
     */
    pid_t _child_pid  { _UNINIT_PID };
    /**
     * @brief If user supplied subcommand args after `--`, launches this as
     *   child process X client to be handled in main queue.
     * @ingroup cli_subcommand
     */
    void _startSubcommandClient();
    /**
     * @brief Constant representing no events in a `pollfd`(`poll(2)`) events
     *   field, named in style of other events constants.
     * @ingroup socket_polling
     */
    static constexpr short _POLLNONE {};
    /**
     * @brief Stores socket polling status of all active file descriptors in
     *   array passable as `poll(2)` param `fds`.
     * @ingroup socket_polling
     */
    std::vector< ::pollfd > _pfds;
    /**
     * @brief Maps socket polling status (referenced by index in
     *   [_pfds](#_pfds)) to open file descriptors.
     * @ingroup socket_polling
     */
    // Needs to be iterable to zip values into _pfds, so std::map chosen over
    //   std::unordered_map
    std::map< int, size_t > _pfds_i_by_fd;
    /**
     * @brief Returns error message describing `POLL*` revents constant
     *   (`poll(2)`), or `std::nullopt` if no error.
     * @param fd socket file descriptor
     * @return error message describing `POLL*` revents constant, or
     *   `std::nullopt` if no error
     * @note `POLLHUP` not considered a #Connection ending error, as a socket
     *   that polls as such may still have data left to be read (see `poll(2)`).
     *   Instead we rely on a later read of that socket returning 0/EOF to
     *   trigger closing the #Connection.
     * @ingroup socket_polling
     */
    std::optional< std::string_view >
    _socketPollError( const int fd );
    /**
     * @brief Adds socket file descriptor to polling group.
     * @param fd socket file descriptor
     * @param events `POLL*` events constant (`poll(2)`) to check for
     * @ingroup socket_polling
     */
    void _addSocketToPoll( const int fd, const short events = _POLLNONE );
    /**
     * @brief First stage of client queue loop; updates which events to poll
     *   for based on [Connection](#Connection) socket buffer status.
     * @ingroup socket_polling
     */
    void _updatePollFlags();
    /**
     * @brief Checks if socket has polled as read-ready.
     * @param fd socket file descriptor
     * @return whether socket has polled as read-ready
     * @ingroup socket_polling
     */
    inline bool _socketReadReady( const int fd ) {
        return ( _pfds.at( _pfds_i_by_fd.at( fd ) ).revents & POLLIN );
    }
    /**
     * @brief Checks if socket has polled as write-ready.
     * @param fd socket file descriptor
     * @return whether socket has polled as write-ready
     * @ingroup socket_polling
     */
    inline bool _socketWriteReady( const int fd ) {
        return ( _pfds.at( _pfds_i_by_fd.at( fd ) ).revents & POLLOUT );
    }
    /**
     * @brief Final stage of client queue loop; surveys socket polling results
     *   for every [Connection](#Connection) to: receive new messages,
     *   parse/log/send currently buffered messages, close any connections that
     *   failed or detected EOF in either socket, and open a new connection if
     *   [_listener_fd](#_listener_fd) is read-ready.
     * @ingroup socket_polling
     */
    void _processPolledSockets();
    /**
     * @brief Constant to set `backlog` param for `listen(2)`.
     * @ingroup main_client_queue
     */
    static constexpr int _MAX_PENDING_CONNECTIONS { 20 };
    /**
     * @brief Sentinel value indicating uninitialized file descriptor.
     * @ingroup main_client_queue
     */
    static constexpr int _UNINIT_FD { -1 };
    /**
     * @brief File descriptor of socket used to `listen(2)` for and `accept(2)`
     *   connections from clients.
     * @ingroup main_client_queue
     */
    int _listener_fd  { _UNINIT_FD };
    /**
     * @brief Active connections indexed by ID number.
     * @ingroup main_client_queue
     */
    std::unordered_map< int, Connection > _connections;
    /**
     * @brief Set up server by creating valid `listen(2)`ing socket.
     * @ingroup main_client_queue
     */
    void _listenForClients();
    /**
     * @brief `accept(2)` client on `listen(2)`ing socket.
     * @param[out] conn connection in which to populate `client_fd` and
     *   `client_desc`
     * @return `true` on success, `false` on failure
     * @ingroup main_client_queue
     */
    bool _acceptClient( Connection* conn );
    /**
     * @brief `connect(2)` to real X server on behalf of a client.
     * @return file descriptor of new socket, or -1 on failure
     * @ingroup main_client_queue
     */
    int  _connectToServer();
    /**
     * @brief Opens new [Connection](#Connection) and adds it to socket polling,
     *   populating the client data with [_acceptClient](#_acceptClient), and
     *   server data with [_connectToServer](#_connectToServer).
     * @ingroup main_client_queue
     */
    void _openConnection();
    /**
     * @brief Closes all members of [_connections](#_connections) with matching
     *   `ids`.
     * @param ids array of all connection ids to close
     * @ingroup main_client_queue
     */
    void _closeConnections( const std::vector< int >& ids );
    /**
     * @brief Begin operation.
     * @return `EXIT_SUCCESS` by default, `EXIT_FAILURE` on error. If using
     *   subcommand child process but not `--keeprunning`, child return value or
     *   signal value + 128 if child stopped by signal.
     * @ingroup main_client_queue
     */
    int  _processClientQueue();
    /**
     * @brief X11 protocol message parser/logger.
     */
    X11ProtocolParser _parser;
};


#endif  // PROXYX11SERVER_HPP
