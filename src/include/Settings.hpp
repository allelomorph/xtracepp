#ifndef SETTINGS_HPP
#define SETTINGS_HPP


#include <getopt.h>  // option required_argument no_argument
#include <stdio.h>   // FILE stdout

#include <cstdint>

#include <vector>
#include <string_view>


class Settings {
private:
    enum LongOptId {
        LO_DEFAULT = 0,
//        LO_VERSION,
        LO_HELP,
    };
    inline static int _long_only_option {};
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
//        { "version",              no_argument,       &_long_only_option, LO_VERSION },
        { NULL,                   0,                 NULL,               0 }
    };
    static constexpr std::string_view _optstring { "+d:D:kewo:umvrp" };

    void _recordFileStreamBufferDefaults();
    void _restoreFileStreamBufferDefaults();

    int    _log_fs_mode {};
    size_t _log_fs_buffer_sz {};

public:
    bool readwritedebug           { false };
    bool copyauth                 { true };
    bool keeprunning              { false };
    bool denyallextensions        { true };   // TBD temp default until extensions implemented
    bool multiline                { false };
    bool verbose                  { false };
    bool relativetimestamps       { false };
    bool prefetchatoms            { false };
    bool unbuffered               { false };

    const char*   log_path        { nullptr };
    ::FILE*       log_fs          { ::stdout };
    const char*   out_displayname { nullptr };
    const char*   in_displayname  { nullptr };

    const char*   process_name    { nullptr };  // argv[0]
    const char**  subcmd_argv     { nullptr };  // argv after --
    int           subcmd_argc     {};           // subcmd_argv arg ct

    // reference points to interpret server millisecond timestamps as real times
    uint32_t ref_TIMESTAMP {};
    ::time_t ref_unix_time {};

    ~Settings();

    void parseFromArgv( const int argc, const char* argv[] );
};


#endif  // SETTINGS_HPP
