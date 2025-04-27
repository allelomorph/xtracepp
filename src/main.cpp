#include "ProxyX11Server.hpp"

// TBD char* const* argv instead of const char* const* argv due to needs of getopt_long
int main(const int argc, char* const* argv) {
    ProxyX11Server server {};
    server.init(argc, argv);
    return server.run();
}
