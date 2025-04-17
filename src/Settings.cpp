#include <unistd.h>  // optarg optind
#include <getopt.h>  // getopt_long
#include <stdlib.h>  // strtoll exit

#include <cassert>

#include <iostream>
#include <filesystem>

#include "Settings.hpp"


Settings::~Settings() {
    log_ofs.close();
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
        case 'f':
            out_authfile = optarg;
            break;
        case 'F':
            in_authfile = optarg;
            break;
        case 'c':
            copyauth = true;
            break;
        case 'n':
            copyauth = false;
            break;
        case 'W':
            waitforclient = true;
            break;
        case 's':
            stopwhennone = true;
            break;
        case 'k':
            stopwhennone = false;
            break;
        case 'e':
            denyallextensions = true;
            break;
        case 'w':
            readwritedebug = true;
            break;
        case 'm':
            maxshownlistlen = std::strtoll( optarg, NULL, 0 );
            break;
        case 'i':
            interactive = true;
            break;
        case 'b':
            buffered = true;
            break;
        case 'o':
            if ( log_path != nullptr ) {
                log_ofs.close();
                std::filesystem::remove( log_path );
                std::cerr << "Multiple -o options!\n";
                exit(EXIT_FAILURE);
            }
            // if( strcmp(optarg,"-") == 0 )
            //     out = stdout;
            // TBD consider making log_path into filesystem::path in try/except
            //     and rethrow exceptions indicating file path formatting errors
            log_path = optarg;
            log_ofs.open( log_path );
            if ( !log_ofs.good() ) {
                // TBD exception / message
                exit(EXIT_FAILURE);
            }
            // https://stackoverflow.com/q/366955/
            log_os.rdbuf( log_ofs.rdbuf() );
            break;
        case '\0':
            switch( _long_only_option ) {
            case LO_HELP:
                std::cout << argv[0] <<
                    ": Dump all X protocol data being tunneled from a fake X display to a real one.\n";
                std::cout << "usage: " << argv[0] <<
                    " [options] [[--] command args ...]\n"
                    "--display, -d <display to connect to>\n"
                    "--fakedisplay, -D <display to fake>\n"
                    "--copyauthentication, -c	Copy credentials\n"
                    "--nocopyauthentication, -n	Do not copy credentials\n"
                    "--authfile, -f <file instead of ~/.Xauthority to get credentials from>\n"
                    "--newauthfile, -F <file instead of ~/.Xauthority to put credentials in>\n"
                    "--waitforclient, -W		wait for connection even if command terminates\n"
                    "--stopwhendone, -s		Return when last client disconnects\n"
                    "--keeprunning, -k		Keep running\n"
                    "--denyextensions, -e		Fake unavailability of all extensions\n"
                    "--readwritedebug, -w		Print amounts of data read/sent\n"
                    "--maxlistlength, -m <maximum number of entries in each list shown>\n"
                    "--outfile, -o <filename>	Output to file instead of stdout\n"
                    "--buffered, -b			Do not output every line but only when buffer is full\n";
                exit(EXIT_SUCCESS);
            case LO_VERSION:
                // puts(PACKAGE " version " VERSION);
                exit(EXIT_SUCCESS);
            case LO_TIMESTAMPS:
                print_timestamps = true;
                break;
            case LO_RELTIMESTAMPS:
                print_reltimestamps = true;
                break;
            case LO_UPTIMESTAMPS:
// #ifndef HAVE_MONOTONIC_CLOCK
//                 fprintf(stderr, "--monotonic-timestamps not supported as clock_gettime(MONOTONIC_CLOCK, ) was not available at compile time\n");
//                 exit(EXIT_FAILURE);
// #else
//                 if (sysconf(_SC_MONOTONIC_CLOCK) < 0) {
//                     fprintf(stderr, "--monotonic-timestamps not supported on this system\n");
//                     exit(EXIT_FAILURE);
//                 }
//                 print_uptimestamps = true;
// #endif
                break;
            case LO_PRINTCOUNTS:
                print_counts = true;
                break;
            case LO_PRINTOFFSETS:
                print_offsets = true;
                break;
            }
            break;
        case ':':
        case '?':
        default:
            exit(EXIT_FAILURE);
        }
    }
}
