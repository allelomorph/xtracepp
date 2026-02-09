/**
 * @file main.cpp
 */

#include "ProxyX11Server.hpp"


/**
 * @brief Entry point.
 * @param argc count of CLI tokens
 * @param argv array of CLI tokens
 * @return
 *   - normal exit: `EXIT_SUCCESS`, or `EXIT_FAILURE` on error
 *   - stopped by signal: signal value +
 *     [SIGNAL_RETVAL_OFFSET](#ProxyX11Server::SIGNAL_RETVAL_OFFSET)
 *   - using child process (without `--keeprunning`): child return value
 *   - using child process (without `--keeprunning`,) child stopped by signal:
 *     signal value + [SIGNAL_RETVAL_OFFSET](#ProxyX11Server::SIGNAL_RETVAL_OFFSET)
 */
int main( const int argc, const char* argv[] ) {
    ProxyX11Server server { argc, argv };
    return server.run();
}
