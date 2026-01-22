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
    /** @brief Allows for toggling of extension flags by id rather than
     *    member name. */
    std::unordered_map< int, bool& > _flags_by_id {
        { BIG_REQUESTS, this->big_requests }
    };

public:
    /**
     * @brief Unique extension numbers for compile time identification (as
     *   opposed to major opcodes assigned by X server at run time).
     */
    enum Ids {
        GENERICEVENT, SHAPE, MIT_SHM, XINPUT, XTEST, BIG_REQUESTS, SYNC,
        XKEYBOARD, XC_MISC, XFIXES, RENDER, RANDR, XINERAMA, COMPOSITE,
        DAMAGE, DOUBLE_BUFFER, RECORD, PRESENT, DRI3, X_RESOURCE, XVIDEO,
        GLX, XFREE86_VIDMODE
    };
    /** @brief Flag for use of BIG-REQUESTS; when true enables extension
     *    [encoding] and an increase in maximum request size.
     *  [encoding]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html#Encoding */
    bool big_requests {};

    /**
     * @brief Activate an extension by [id](#Ids).
     * @param ext_id extension [id](#Ids)
     */
    inline void
    activate( const int ext_id ) {
        auto it { _flags_by_id.find( ext_id ) };
        if ( it != _flags_by_id.end() )
            it->second = true;
    }
    /**
     * @brief Check by [id](#Ids) if extension is active.
     * @param ext_id extension [id](#Ids)
     * @return whether extension is active
     */
    inline bool
    active( const int ext_id ) {
        auto it { _flags_by_id.find( ext_id ) };
        if ( it != _flags_by_id.end() )
            return it->second;
        return false;
    }
    /**
     * @brief Deactivate an extension by [id](#Ids).
     * @param ext_id extension [id](#Ids)
     */
    inline void
    deactivate( const int ext_id ) {
        auto it { _flags_by_id.find( ext_id ) };
        if ( it != _flags_by_id.end() )
            it->second = false;
    }
};


#endif  // EXTENSIONS_HPP
