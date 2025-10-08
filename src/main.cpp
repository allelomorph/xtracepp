#include "ProxyX11Server.hpp"

int main( const int argc, const char* argv[] ) {
    ProxyX11Server server {};
    server.init( argc, argv );
    return server.run();
}
