#ifndef EXTENSIONS_HPP
#define EXTENSIONS_HPP

/**
 * @file Extensions.hpp
 */

#include <string_view>
#include <unordered_map>

#include <cstdint>


/**
 * @brief Tracks activation of extension features, both by unique [ids](#Ids)
 *   and by named boolean members.
 */
class Extensions {
private:
    /** @brief Allows for toggling of extension flags by extension name rather
     *    than member name. */
    std::unordered_map< std::string_view, bool& > _flags_by_id {
        { "BIG-REQUESTS", this->big_requests }
    };

public:
    /** @brief Flag for use of BIG-REQUESTS; when true enables extension
     *    [encoding] and an increase in maximum request size.
     *  [encoding]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding */
    bool big_requests {};

    /**
     * @brief Activate an extension by name.
     * @param name extension name
     */
    inline void
    activate( const std::string_view& name ) {
        auto it { _flags_by_id.find( name ) };
        if ( it != _flags_by_id.end() )
            it->second = true;
    }
    /**
     * @brief Check by name if extension is active.
     * @param name extension name
     * @return whether extension is active
     */
    inline bool
    active( const std::string_view& name ) {
        auto it { _flags_by_id.find( name ) };
        if ( it != _flags_by_id.end() )
            return it->second;
        return false;
    }
    /**
     * @brief Deactivate an extension by name.
     * @param name extension name
     */
    inline void
    deactivate( const std::string_view& name ) {
        auto it { _flags_by_id.find( name ) };
        if ( it != _flags_by_id.end() )
            it->second = false;
    }
};


#endif  // EXTENSIONS_HPP
