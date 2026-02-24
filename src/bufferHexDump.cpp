#include <algorithm>          // min
#include <string>

#include <cstdint>
#include <cassert>
#include <cctype>             // isprint

#include <fmt/format.h>

#include "bufferHexDump.hpp"


std::string bufferHexDump( const uint8_t* data, const size_t sz ) {
    assert( data != nullptr );

    static constexpr size_t BYTES_PER_ROW   { 16 };
    static constexpr size_t BYTES_PER_GROUP { 8 };

    std::string hex_dump;
    const uint8_t* _data { data };
    for ( size_t bytes_to_parse { sz }, address_index {};
          bytes_to_parse > 0; address_index += BYTES_PER_ROW ) {
        std::string group1, as_ascii;
        for ( size_t i {}, group_sz {
                std::min(bytes_to_parse, BYTES_PER_GROUP ) };
             i < group_sz; ++i, ++_data, --bytes_to_parse ) {
            group1   += fmt::format(
                "{:02x}{}", *_data, ( i < group_sz - 1 ) ? " " : "");
            as_ascii += fmt::format(
                "{:c}", ( std::isprint( *_data ) ) ? *_data : '.');
        }
        std::string group2;
        for (size_t i {}, group_sz {
                std::min( bytes_to_parse, BYTES_PER_GROUP ) };
             i < group_sz; ++i, ++_data, --bytes_to_parse ) {
            group2   += fmt::format(
                "{:02x}{}", *_data, ( i < group_sz - 1 ) ? " " : "");
            as_ascii += fmt::format(
                "{:c}", std::isprint( *_data ) ? *_data : '.');
        }
        hex_dump += fmt::format( "{:08x}  {: <23}  {: <23}  {}\n",
                                 address_index, group1, group2, as_ascii );
    }
    return hex_dump;
}
