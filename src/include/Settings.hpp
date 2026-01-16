#ifndef SETTINGS_HPP
#define SETTINGS_HPP

/**
 * @file Settings.hpp
 */

#include <getopt.h>  // option required_argument no_argument
#include <stdio.h>   // FILE stdout

#include <cstdint>

#include <vector>
#include <string_view>


/**
 * @brief Encapsulates parsing of `argv` from [main](#main) into user settings.
 */
class Settings {
private:
    /**
     * @brief Constants used to indentify longopt-only options in `::option`
     *   structs passed to `getopt_long(3)`.
     * @ingroup getopt_long
     */
    enum _LongOptId {
        LO_DEFAULT = 0,
        LO_HELP,
    };
    /**
     * @brief Flag set to a [_LongOptId](#_LongOptId) value by `getopt_long(3)`
     *   when handling a longopt.
     * @ingroup getopt_long
     */
    inline static int _long_only_option {};
    /**
     * @brief Stores C array of `::option` structs to pass to `getopt_long(3)`.
     * @ingroup getopt_long
     */
    inline static const std::vector< ::option > _longopts {
        { "display",              required_argument, NULL,              'd' },
        { "proxydisplay",         required_argument, NULL,              'D' },
        { "keeprunning",          no_argument,       NULL,              'k' },
        { "denyextensions",       no_argument,       NULL,              'e' },
        { "readwritedebug",       no_argument,       NULL,              'w' },
        { "outfile",              required_argument, NULL,              'o' },
        { "unbuffered",           no_argument,       NULL,              'u' },
        { "multiline",            no_argument,       NULL,              'm' },
        { "verbose",              no_argument,       NULL,              'v' },
        { "relativetimestamps",   no_argument,       NULL,              'r' },
        { "prefetchatoms",        no_argument,       NULL,              'p' },
        { "help",                 no_argument,       &_long_only_option, LO_HELP },
        { NULL,                   0,                 NULL,               0 }
    };
    /**
     * @brief Stores C string of legitimate option characters to pass to
     *   `getopt_long(3)`.
     * @ingroup getopt_long
     */
    static constexpr std::string_view _optstring { "+d:D:kewo:umvrp" };
    /**
     * @brief Records buffering state of [log_fs](#log_fs) before applying user
     *   settings.
     * @ingroup log_fs_buffering
     */
    void _recordFileStreamBufferDefaults();
    /**
     * @brief Restores buffering state of [log_fs](#log_fs) before applying user
     *   settings.
     * @ingroup log_fs_buffering
     */
    void _restoreFileStreamBufferDefaults();
    /**
     * @brief Used by [_recordFileStreamBufferDefaults](#_recordFileStreamBufferDefaults)
     *   to store buffering mode of [log_fs](#log_fs).
     * @ingroup log_fs_buffering
     */
    int    _log_fs_mode {};
    /**
     * @brief Used by [_recordFileStreamBufferDefaults](#_recordFileStreamBufferDefaults)
     *   to store buffer size of [log_fs](#log_fs).
     * @ingroup log_fs_buffering
     */
    size_t _log_fs_buffer_sz {};

public:
    /**
     * @brief Toggles explicit logging of every buffer and socket read/write operation.
     */
    bool readwritedebug           { false };
    /**
     * @brief Toggles copying authorization data from real X server display.
     */
    bool copyauth                 { true };
    /**
     * @brief Toggles indefinte packet logging, independent of subcommand client.
     */
    bool keeprunning              { false };
    /**
     * @brief Toggles disabling of all X extensions.
     */
    bool denyallextensions        { true };   // TBD temp default until extensions implemented
    /**
     * @brief Toggles logging most protocol structs in multiline format, with
     *   one line per member.
     */
    bool multiline                { false };
    /**
     * @brief Toggles more verbose logging.
     */
    bool verbose                  { false };
    /**
     * @brief Toggles interpretation of X server TIMESTAMPs in context of system time.
     */
    bool relativetimestamps       { false };
    /**
     * @brief Toggles fetching of all atom strings from real X server in
     *   contiguous range of ATOMs 1 to n; greatly reduces indcience of
     *   "unknown ATOM" in logs.
     */
    bool prefetchatoms            { false };
    /**
     * @brief Disables buffering on [log_fs](#log_fs).
     */
    bool unbuffered               { false };
    /**
     * @brief Full path to log file, if not using `::stdout` or `::stderr`.
     */
    const char*   log_path        { nullptr };
    /**
     * @brief Log file stream.
     */
    ::FILE*       log_fs          { ::stdout };
    /**
     * @brief X display name string for real X server.
     * @see [DisplayInfo](#DisplayInfo)
     */
    const char*   out_displayname { nullptr };
    /**
     * @brief X display name string for proxy server.
     * @see [DisplayInfo](#DisplayInfo)
     */
    const char*   in_displayname  { nullptr };
    /**
     * @brief Application name from `argv[0]`, for use in error messages.
     */
    const char*   process_name    { nullptr };
    /**
     * @brief Subcommand tokens (`argv` after `--`).
     */
    const char**  subcmd_argv     { nullptr };
    /**
     * @brief Subcommand token count (length of `argv` after `--`).
     */
    int           subcmd_argc     {};
    /**
     * @brief Reference X server TIMESTAMP (milliseconds since ?) to compare
     *   against [ref_unix_time](#ref_unix_time) when calculating
     *   [relativetimestamps](#relativetimestamps).
     */
    uint32_t ref_TIMESTAMP {};
    /**
     * @brief Reference unix timestamp (seconds since unix epoch) to compare
     *   against [ref_TIMESTAMP](#ref_TIMESTAMP) when calculating
     *   [relativetimestamps](#relativetimestamps).
     */
    ::time_t ref_unix_time {};
    /**
     * @brief Flushes [log_fs](#log_fs), closes it if opened from file,
     *   restores original system file stream buffering settings.
     */
    ~Settings();
    /**
     * @brief Parses `argv` from [main](#main) to populate data members.
     * @param argc `argc` from [main](#main); count of CLI tokens
     * @param argv `argv` from [main](#main); C array of CLI tokens
     */
    void parseFromArgv( const int argc, const char* argv[] );
};


#endif  // SETTINGS_HPP
