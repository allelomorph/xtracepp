#include <unistd.h>      // optarg optind _POSIX_MONOTONIC_CLOCK sysconf
#include <getopt.h>      // getopt_long
#include <stdlib.h>      // strtoll ::exit
#include <stdio.h>       // fclose fopen setvbuf _IONBF _IOLBF _IOFBF
#include <stdio_ext.h>   // __flbf __fbufsize

#include <cassert>

#include <iostream>
#include <filesystem>
#include <string_view>
#include <string>
#include <algorithm>  // min

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
    std::unordered_set< std::string_view > enabled_extensions;
    std::unordered_set< std::string_view > disabled_extensions;
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
            assert( optarg != nullptr );
            out_displayname = optarg;
            break;
        case 'D':
            assert( optarg != nullptr );
            in_displayname = optarg;
            break;
        case 'k':
            keeprunning = true;
            break;
        case 'e': {
            assert( optarg != nullptr );
            if ( !enabled_extensions.empty() ) {
                fmt::println( ::stderr, "{}: --denyextensions/-e and "
                              "--allowextensions/-E cannot be in same command",
                              process_name );
                ::exit( EXIT_FAILURE );
            }
            if ( const std::string_view e_name { optarg };
                 e_name == _ALL ) {
                disabled_extensions = _recognized_extensions;
            } else {
                validateExtensionName( e_name );
                disabled_extensions.emplace( e_name );
            }
        }   break;
        case 'E': {
            assert( optarg != nullptr );
            if ( !disabled_extensions.empty() ) {
                fmt::println( ::stderr, "{}: --denyextensions/-e and "
                              "--allowextensions/-E cannot be in same command",
                              process_name );
                ::exit( EXIT_FAILURE );
            }
            const std::string_view e_name { optarg };
            validateExtensionName( e_name );
            enabled_extensions.emplace( e_name );
        }   break;
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
            assert( optarg != nullptr );
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
        case 's':
            systemtimeformat = true;
            break;
        case 'p':
            prefetchatoms = true;
            break;
        case '\0':
            switch( _long_only_option ) {
            case LO_HELP:
                fmt::print( ::stderr, _help_msg, process_name, process_name );
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
    for ( const std::string_view& e_name : disabled_extensions )
        _allowed_extensions.erase( e_name );
    // disable any recognized extensions not explicitly enabled, unless
    //   --allowextensions/-E was not used at all
    if ( !enabled_extensions.empty() ) {
        for ( const std::string_view& e_name : _recognized_extensions ) {
            if ( enabled_extensions.find( e_name ) == enabled_extensions.end() )
                _allowed_extensions.erase( e_name );
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

void Settings::validateExtensionName( const std::string_view& name ) {
    bool recognized {};
    int max_match_score {};
    std::string partial_matches;
    for ( const std::string_view& re_name : _recognized_extensions ) {
        if ( name == re_name ) {
            recognized = true;
            break;
        }
        int match_score {};
        for ( int i {}, min_len ( std::min( name.size(), re_name.size() ) );
              i < min_len && name.at( i ) == re_name.at( i ); ++i ) {
            ++match_score;
        }
        if ( match_score > max_match_score ) {
            partial_matches = fmt::format( "{:?}", re_name );
            max_match_score = match_score;
        } else if ( match_score == max_match_score ) {
            partial_matches += fmt::format( ",{:?}", re_name );
        }
    }
    if ( !recognized ) {
        fmt::println( ::stderr, "{}: unrecognized extension name {:?}, {}",
                      process_name, name,
                      max_match_score == 0 ?
                      fmt::format( "expected among: {}", _VALID_EXT_NAMES ) :
                      fmt::format( "partial matches: {}", partial_matches ) );
        ::exit( EXIT_FAILURE );
    }
}
