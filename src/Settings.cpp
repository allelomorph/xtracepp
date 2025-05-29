#include <unistd.h>  // optarg optind _POSIX_MONOTONIC_CLOCK sysconf
#include <getopt.h>  // getopt_long
#include <stdlib.h>  // strtoll exit
#include <stdio.h>   // fclose fopen

#include <cassert>

#include <iostream>
#include <filesystem>
#include <string_view>

#include "Settings.hpp"
#include "errors.hpp"


Settings::~Settings() {
    fflush( log_fs );
    fclose( log_fs );
}

void Settings::parseFromArgv(const int argc, char* const* argv) {
    assert( argc > 0 );
    assert( argv != nullptr );

    for( char c ( getopt_long( argc, argv, _optstring.data(), _longopts, NULL ) );
        c != -1;
        c = getopt_long( argc, argv, _optstring.data(), _longopts, NULL ) ) {
        switch( c ) {
        case 'd':
            out_displayname = optarg;
            break;
        case 'D':
            in_displayname = optarg;
            break;
        // case 'f':
        //     out_authfile = optarg;
        //     break;
        // case 'F':
        //     in_authfile = optarg;
        //     break;
        // case 'c':
        //     copyauth = true;
        //     break;
        // case 'n':
        //     copyauth = false;
        //     break;
        // case 'W':
        //     waitforclient = true;
        //     break;
        // case 's':
        //     stopifnoactiveconnx = true;
        //     break;
        // case 'k':
        //     stopifnoactiveconnx = false;
        //     break;
        case 'e':
            denyallextensions = true;
            break;
        case 'w':
            readwritedebug = true;
            break;
        // case 'm':
        //     maxshownlistlen = std::strtoll( optarg, NULL, 0 );
        //     break;
        // case 'i':
        //     interactive = true;
        //     break;
        // case 'b':
        //     buffered = true;
        //     break;
        case 'o':
            if ( log_path != nullptr ) {
                std::filesystem::remove( log_path );
                std::cerr << argv[0] << ": -o option may only be used once" << std::endl;
                exit( EXIT_FAILURE );
            }
            // TBD consider making log_path into filesystem::path in try/except
            //     and rethrow exceptions indicating file path formatting errors
            log_path = optarg;
            assert( log_path != nullptr && log_path[0] != '\0' );
            // getopt_long edge case: parsing an option that takes arguments,
            //   followed by another option with no whitespace in between, eg:
            //   " -o--help " (optarg == "--help") or " -f-k " (optarg == "-k")
            // TBD error quit if any optarg starts with '-', not just log_path
            if ( log_path[0] == '-' ) {
                std::cerr << argv[0] <<
                    ": file path passed to -o option may not begin with '-' to "
                    "prevent option parsing errors" << std::endl;
                exit( EXIT_FAILURE );
            }
            log_fs = fopen( log_path, "w" );
            if ( log_fs == nullptr ) {
                std::cerr << argv[0] <<
                    ": could not open log file \"" << log_path << "\": " <<
                    errors::system::message( "" ) << std::endl;
                exit( EXIT_FAILURE );
            }
            break;
        case '\0':
            switch( _long_only_option ) {
            case LO_HELP:
                std::cout << argv[0] <<
                    ": Intercept, log, and modify (based on user options) "
                    "packet data going between X server and clients\n";
                std::cout <<
                    "usage: " << argv[0] << " [options] [[--] command args ...]\n" <<
                    "--display, -d <display name representing actual X server>\n"
                    "--proxydisplay, -D <proxy display name representing this server>\n"
                    // "--copyauthentication, -c	Copy credentials\n"
                    // "--nocopyauthentication, -n	Do not copy credentials\n"
                    // "--authfile, -f <file instead of ~/.Xauthority to get credentials from>\n"
                    // "--newauthfile, -F <file instead of ~/.Xauthority to put credentials in>\n"
                    // "--waitforclient, -W		wait for connection even if command terminates\n"
                    // "--stopwhendone, -s		Return when last client disconnects\n"
                    // "--keeprunning, -k		Keep running\n"
                    "--denyextensions, -e		Fake unavailability of all extensions\n"
                    "--readwritedebug, -w		Print amounts of data read/sent\n"
                    // "--maxlistlength, -m <maximum number of entries in each list shown>\n"
                    "--outfile, -o <filename>	Output to file instead of stdout\n";
                    // "--buffered, -b			Do not output every line but only when buffer is full\n";
                exit( EXIT_SUCCESS );
            // case LO_VERSION:
            //     // puts(PACKAGE " version " VERSION);
            //     exit( EXIT_SUCCESS );
            // case LO_TIMESTAMPS:
            //     print_timestamps = true;
            //     break;
            // case LO_RELTIMESTAMPS:
            //     print_reltimestamps = true;
            //     break;
//             case LO_UPTIMESTAMPS:
//                 // Relevant POSIX feature test macro, see:
//                 //   - https://stackoverflow.com/a/50245503
//                 //   - man 7 posixoptions
// #ifndef _POSIX_MONOTONIC_CLOCK
//                 std::cerr << argv[0] << ": --monotonic-timestamps not supported: "
//                     "clock_gettime(2) clockid MONOTONIC_CLOCK was not available at compile time" <<
//                     std::endl;
//                 exit( EXIT_FAILURE );
// #elif   _POSIX_MONOTONIC_CLOCK == 0
//                 // macro defined as 0 necessitates run time option check, see:
//                 //   - https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/unistd.h.html
//                 //     "Constants for Options and Option Groups"
//                 if ( sysconf( _SC_MONOTONIC_CLOCK ) < 0 ) {
//                     std::cerr << argv[0] << ": --monotonic-timestamps not supported: "
//                         "clock_gettime(2) clockid MONOTONIC_CLOCK was not available at run time" <<
//                         std::endl;
//                     exit( EXIT_FAILURE );
//                 }
// #endif
//                 print_uptimestamps = true;
//                 break;
            // case LO_PRINTCOUNTS:
            //     print_counts = true;
            //     break;
            // case LO_PRINTOFFSETS:
            //     print_offsets = true;
            //     break;
            }
            break;
        case ':':
        case '?':
        default:
            exit( EXIT_FAILURE );
        }
    }

    if ( optind < argc && argv[optind] != std::string_view( "--" ) ) {
        subcmd_argc = argc - optind;
        subcmd_argv = argv + optind;
    }
}
