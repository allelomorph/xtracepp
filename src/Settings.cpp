#include <unistd.h>      // optarg optind _POSIX_MONOTONIC_CLOCK sysconf
#include <getopt.h>      // getopt_long
#include <stdlib.h>      // strtoll ::exit
#include <stdio.h>       // fclose fopen setvbuf _IONBF _IOLBF _IOFBF
#include <stdio_ext.h>   // __flbf __fbufsize

#include <cassert>

#include <iostream>
#include <filesystem>
#include <string_view>

#include <fmt/format.h>

#include "Settings.hpp"
#include "errors.hpp"


Settings::~Settings() {
    ::fflush( log_fs );
    if ( log_fs == ::stdout || log_fs == ::stderr ) {
        _restoreFileStreamBufferDefaults();
    } else {
        fclose( log_fs );
    }
}

void Settings::parseFromArgv( const int argc, const char* argv[] ) {
    assert( argc > 0 );
    assert( argv != nullptr );

    process_name = argv[0];

    static constexpr std::string_view help_msg {
        R"({}: intercept, log, and modify (based on user options) packet data going between X server and clients
  (usage: {} [options...] [-- subcommand args...]
  options:
    --display,            -d <display name>  : provide libX11 formatted display name of real X server
    --proxydisplay,       -D <display name>  : provide libX11 formatted display name of this proxy server
    --keeprunning,        -k                 : continue monitoring traffic after subcommand client exits
    --denyextensions,     -e                 : disable use of all X extensions
    --readwritedebug,     -w                 : print amounts of data read/sent
    --outfile,            -o <file path>     : output to file instead of stdout
    --unbuffered,         -u                 : deactivate stream buffering for output
    --multiline,          -m                 : break log lines along nested groupings of data
    --verbose,            -v                 : print all data fields of every packet + alternate data formatting
    --relativetimestamps, -r                 : X server timestamps interpreted against system time
    --prefetchatoms,      -p                 : first fetch already interned strings to reduce unrecognized ATOMs
)" };

    for ( char c ( ::getopt_long( argc, const_cast< char* const* >( argv ),
                                  _optstring.data(), _longopts.data(), nullptr ) );
          c != -1;
          c = ::getopt_long( argc, const_cast< char* const* >( argv ),
                             _optstring.data(), _longopts.data(), nullptr ) ) {

        if ( optarg != nullptr && optarg[0] == '-' ) {
            fmt::println( ::stderr, "{}: option arguments may not begin with '-' "
                          "to prevent option parsing errors", process_name );
            ::exit( EXIT_FAILURE );
        }
        switch ( c ) {
        case 'd':
            out_displayname = optarg;
            break;
        case 'D':
            in_displayname = optarg;
            break;
        case 'k':
            keeprunning = true;
            break;
        case 'e':
            denyallextensions = true;
            break;
        case 'w':
            readwritedebug = true;
            break;
        case 'u':
            unbuffered = true;
            break;
        case 'o':
            if ( log_path != nullptr ) {
                std::filesystem::remove( log_path );
                fmt::println( ::stderr, "{}: -o option may only be used once",
                              process_name );
                ::exit( EXIT_FAILURE );
            }
            log_path = optarg;
            assert( log_path != nullptr && log_path[0] != '\0' );
            log_fs = fopen( log_path, "w" );
            if ( log_fs == nullptr ) {
                fmt::println( ::stderr, "{}: could not open log file {:?}, {}",
                              process_name, log_path,
                              errors::system::message( "fopen" ) );
                ::exit( EXIT_FAILURE );
            }
            break;
        case 'm':
            multiline = true;
            break;
        case 'v':
            verbose = true;
            break;
        case 'r':
            relativetimestamps = true;
            break;
        case 'p':
            prefetchatoms = true;
            break;
        case '\0':
            switch( _long_only_option ) {
            case LO_HELP:
                fmt::print( ::stderr, help_msg, process_name, process_name );
                ::exit( EXIT_SUCCESS );
            }
            break;
        case ':':
            [[fallthrough]];
        case '?':
            [[fallthrough]];
        default:
            ::exit( EXIT_FAILURE );
        }
    }

    if ( ::optind < argc && argv[ ::optind ] != std::string_view( "--" ) ) {
        subcmd_argc = argc - optind;
        subcmd_argv = argv + optind;
    }

    _recordFileStreamBufferDefaults();
    if ( unbuffered ) {
        if ( setvbuf( log_fs, nullptr, _IONBF, 0 ) != 0 ) {
            fmt::println( ::stderr, "{}: {}",
                          process_name, errors::system::message( "setvbuf" ) );
            ::exit( EXIT_FAILURE );
        }
    }
}

void Settings::_recordFileStreamBufferDefaults() {
    assert( log_fs == ::stdout || log_fs == ::stderr );
    _log_fs_buffer_sz = ::__fbufsize( log_fs );
    _log_fs_mode = ::__flbf( log_fs ) ? _IOLBF :
        _log_fs_buffer_sz == 0 ? _IONBF : _IOFBF;
}

void Settings::_restoreFileStreamBufferDefaults() {
    assert( log_fs == stdout || log_fs == ::stderr );
    if ( ::setvbuf( log_fs, nullptr, _log_fs_mode, _log_fs_buffer_sz ) != 0 ) {
        fmt::println( ::stderr, "{}: {}",
                      process_name, errors::system::message( "setvbuf" ) );
        ::exit( EXIT_FAILURE );
    }
}
