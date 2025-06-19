#ifndef SETTINGS_HPP
#define SETTINGS_HPP


#include <getopt.h>  // `struct option` required_argument no_argument
#include <stdio.h>   // FILE stdout

#include <cstdint>   // SIZE_MAX

#include <string_view>
#include <iostream>
#include <fstream>

class Settings {
private:
    static constexpr int _OPTION_CT { 22 };
    enum LongOptId {
        LO_DEFAULT = 0,
        LO_TIMESTAMPS,
        LO_RELTIMESTAMPS,
        LO_UPTIMESTAMPS,
        LO_VERSION,
        LO_HELP,
        LO_PRINTCOUNTS,
        LO_PRINTOFFSETS
    };
    inline static int _long_only_option;
    static constexpr struct option _longopts[ _OPTION_CT + 1 ] {
        {"display",              required_argument, NULL,              'd'},
        {"proxydisplay",         required_argument, NULL,              'D'},
        // {"authfile",             required_argument, NULL,              'f'},
        // {"newauthfile",          required_argument, NULL,              'F'},
        // {"copyauthentication",   no_argument,       NULL,              'c'},
        // {"nocopyauthentication", no_argument,       NULL,              'n'},
        // {"waitforclient",        no_argument,       NULL,              'w'},
        // {"stopwhendone",         no_argument,       NULL,              's'},
        // {"keeprunning",          no_argument,       NULL,              'k'},
        {"denyextensions",       no_argument,       NULL,              'e'},
        {"readwritedebug",       no_argument,       NULL,              'w'},
        // {"maxlistlength",        required_argument, NULL,              'm'},  // TBD conlfict with multiline
        {"outfile",              required_argument, NULL,              'o'},
        // {"buffered",             no_argument,       NULL,              'b'},
        // {"interactive",          no_argument,       NULL,              'i'},
        {"multiline",            no_argument,          NULL,           'm'},
        {"verbose",              no_argument,          NULL,           'v'},
        {"help",                 no_argument,       &_long_only_option, LO_HELP},
        {"version",              no_argument,       &_long_only_option, LO_VERSION},
        // {"timestamps",           no_argument,       &_long_only_option, LO_TIMESTAMPS},
        // {"relative-timestamps",  no_argument,       &_long_only_option, LO_RELTIMESTAMPS},
        // {"monotonic-timestamps", no_argument,       &_long_only_option, LO_UPTIMESTAMPS},
        // {"print-counts",         no_argument,       &_long_only_option, LO_PRINTCOUNTS},
        // {"print-offsets",        no_argument,       &_long_only_option, LO_PRINTOFFSETS},
        {NULL,                   0,                 NULL,              0}
    };
    //static constexpr std::string_view _optstring { "+d:D:f:F:cnWskiewm:o:b" };
    static constexpr std::string_view _optstring { "+d:D:ewo:mv" };

public:
    bool readwritedebug           { false };
    // bool copyauth                 { true };
    bool stopifnoactiveconnx      { true };   // stopwhennone
    bool waitforclient            { false };
    bool denyallextensions        { false };
    bool multiline                { false };
    bool verbose                  { false };
    // bool interactive              { false };
    // bool print_timestamps         { false };
    // bool print_reltimestamps      { false };
    // bool print_uptimestamps       { false };
    // bool buffered                 { false };
    // size_t maxshownlistlen        { SIZE_MAX };
    // bool print_counts             { false };
    // bool print_offsets            { false };
    const char*   log_path        { nullptr };
    FILE* log_fs                  { stdout };
    const char*   out_displayname { nullptr };
    const char*   in_displayname  { nullptr };
    // const char* out_authfile      { nullptr };
    // const char* in_authfile       { nullptr };
    char* const*  subcmd_argv     { nullptr };  // argv after --
    int           subcmd_argc     {};           // subcmd_argv arg ct

    ~Settings();

    void parseFromArgv(const int argc, char* const* argv);
};


#endif  // SETTINGS_HPP
