#include "IntermediaryServer.hpp"

// TBD char* const* argv instead of const char* const* argv due to needs of getopt_long
int main(const int argc, char* const* argv) {
    IntermediaryServer server {};
    server.settings.parseFromArgv(argc, argv);
    server.initDisplays();
    server.__debugOutput();
}
