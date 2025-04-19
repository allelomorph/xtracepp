#include "IntermediaryServer.hpp"

// TBD char* const* argv instead of const char* const* argv due to needs of getopt_long
int main(const int argc, char* const* argv) {
    IntermediaryServer server {};
    server.settings.parseFromArgv(argc, argv);
    server.parseDisplayNames();
    // if ( server.settings.copyauth )
    //     copy_authentication();
    // setvbuf(out, NULL, buffered?_IOFBF:_IOLBF, BUFSIZ);
    server.listenForClients();
    server.startClient();
    server.connectToServer();
    server.__debugOutput();
}
