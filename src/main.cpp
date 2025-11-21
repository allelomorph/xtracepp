#include "ProxyX11Server.hpp"

/**
 * @file main.cpp
 */


/**
 * @brief Entry point.
 * @param argc count of CLI tokens
 * @param argv array of CLI tokens
 * @return `EXIT_SUCCESS` by default, `EXIT_FAILURE` on error, or signal value +
 *   128 if stopped by signal. If using subcommand child process but not
 *   `--keeprunning`, child return value or signal value + 128 if child stopped
 *   by signal.
 */
int main( const int argc, const char* argv[] ) {
    ProxyX11Server server {};
    server.init( argc, argv );
    return server.run();
}
