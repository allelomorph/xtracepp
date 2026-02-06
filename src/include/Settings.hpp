#ifndef SETTINGS_HPP
#define SETTINGS_HPP

/**
 * @file Settings.hpp
 */

#include <getopt.h>  // option required_argument no_argument
#include <stdio.h>   // FILE stdout

#include <cstdint>

#include <string>
#include <string_view>
#include <unordered_set>

#include <fmt/format.h>

#include "protocol/extensions/big_requests.hpp"


/**
 * @brief Encapsulates parsing of `argv` from [main](#main) into user settings.
 */
class Settings {
private:
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
    /**
     * @brief Recognized names of X extensions.
     */
    inline static const
    std::unordered_set< std::string_view > _recognized_extensions {
        // TBD disable by omission all unimplemented extensions
        // "Generic Event Extension",
        // "SHAPE",
        // "MIT-SHM",
        // "XInputExtension",
        // "XTEST",
        protocol::extensions::big_requests::name,  // BIG-REQUESTS
        // "SYNC",
        // "XKEYBOARD",
        // "XC-MISC",
        // "XFIXES",
        // "RENDER",
        // "RANDR",
        // "XINERAMA",
        // "Composite",
        // "DAMAGE",
        // "DOUBLE-BUFFER",
        // "RECORD",
        // "Present",
        // "DRI3",
        // "X-Resource",
        // "XVideo",
        // "GLX",
        // "XFree86-VidModeExtension"
    };
    /**
     * @brief #_recognized_extensions as list appropriate for error messages.
     */
    inline static const
    std::string _VALID_EXT_NAMES {
        [](){
            std::string names;
            for ( const std::string_view& name : _recognized_extensions ) {
                names += fmt::format(
                    "{}{:?}", names.empty() ? "" : ",", name );
            }
            return names;
        }() };
    /**
     * @brief Toggles disabling of all X extensions with `--denyextensions`.
     */
    static constexpr std::string_view _ALL { "all" };
    /**
     * @brief Toggles use of individual X extensions, by name.
     */
    std::unordered_set< std::string_view > _allowed_extensions {
        _recognized_extensions };
    /**
     * @brief Exits process with error message if X extension name is not
     *   recognized.
     * @param name extension name
     */
    void _validateExtensionName( const std::string_view& name );
    /**
     * @brief Parses `argv` from [main](#main) to populate data members.
     * @param argc `argc` from [main](#main); count of CLI tokens
     * @param argv `argv` from [main](#main); C array of CLI tokens
     */
    void _parseFromArgv( const int argc, const char* argv[] );

public:
    Settings() = delete;
    /**
     * @brief Default ctor.
     */
    Settings( const int argc, const char* argv[] );
    /**
     * @brief Flushes [log_fs](#log_fs), closes it if opened from file,
     *   restores original system file stream buffering settings.
     */
    ~Settings();
    /**
     * @brief Toggles explicit logging of every buffer and socket read/write operation.
     */
    bool readwritedebug   { false };
    /**
     * @brief Toggles copying authorization data from real X server display.
     */
    bool copyauth         { true };
    /**
     * @brief Toggles indefinte logging, independent of subcommand client.
     */
    bool keeprunning      { false };
    /**
     * @brief Toggles logging most protocol structs in multiline format, with
     *   one line per member.
     */
    bool multiline        { false };
    /**
     * @brief Toggles more verbose logging.
     */
    bool verbose          { false };
    /**
     * @brief Toggles formatting of X protocol TIMESTAMPs in context of system time.
     */
    bool systemtimeformat { false };
    /**
     * @brief Toggles fetching of all atom strings from real X server in
     *   contiguous range of ATOMs 1 to n; greatly reduces indcience of
     *   "unknown ATOM" in logs.
     */
    bool prefetchatoms    { false };
    /**
     * @brief Disables buffering on [log_fs](#log_fs).
     */
    bool unbuffered       { false };
    /**
     * @brief Full path to log file, if not using `::stdout` or `::stderr`.
     */
    const char* log_path { nullptr };
    /**
     * @brief Log file stream.
     */
    ::FILE* log_fs { ::stdout };
    /**
     * @brief X display name string for real X server.
     * @see [DisplayInfo](#DisplayInfo)
     */
    const char* out_displayname { nullptr };
    /**
     * @brief X display name string for proxy server.
     * @see [DisplayInfo](#DisplayInfo)
     */
    const char* in_displayname { nullptr };
    /**
     * @brief Application name from `argv[0]`, for use in error messages.
     */
    const char* process_name { nullptr };
    /**
     * @brief Subcommand tokens (`argv` after `--`).
     */
    const char** subcmd_argv { nullptr };
    /**
     * @brief Subcommand token count (length of `argv` after `--`).
     */
    int subcmd_argc {};
    /**
     * @brief Reference X server TIMESTAMP (milliseconds since ?) to compare
     *   against [ref_unix_time](#ref_unix_time) when calculating
     *   [systemtimeformat](#systemtimeformat).
     */
    uint32_t ref_TIMESTAMP {};
    /**
     * @brief Reference unix timestamp (seconds since unix epoch) to compare
     *   against [ref_TIMESTAMP](#ref_TIMESTAMP) when calculating
     *   [systemtimeformat](#systemtimeformat).
     */
    ::time_t ref_unix_time {};
    /**
     * @brief Returns whether extension name was passed with `--denyextensions`.
     * @param name extension name
     * @return whether extension name was passes with `--denyextensions`
     */
    inline bool extensionDenied( const std::string_view name ) const {
        return _allowed_extensions.find( name ) == _allowed_extensions.end();
    }
};


#endif  // SETTINGS_HPP
