#include "ProxyX11Server.hpp"

// TBD `char* const* argv` required by getopt_long in Settings
int main(const int argc, char* const* argv) {
    ProxyX11Server server {};
    server.init(argc, argv);
    return server.run();
}
