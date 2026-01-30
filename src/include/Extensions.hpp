#ifndef EXTENSIONS_HPP
#define EXTENSIONS_HPP

/**
 * @file Extensions.hpp
 */

#include <string_view>
#include <unordered_map>

#include <cstdint>


/**
 * @brief Tracks activation of extension features, both by extension names
 *   and by named boolean members.
 */
class Extensions {
private:
    enum class _Toggle { Off, On };
    using _ToggleFuncT = void (Extensions::*)( const _Toggle );
    using _CheckFuncT = bool (Extensions::*)();
    struct _MembFuncs {
        _ToggleFuncT toggle_func {};
        _CheckFuncT check_func {};
    };
    inline void _toggleBigRequests( const _Toggle toggle ) {
        big_requests = static_cast< bool >( toggle );
    }
    inline bool _checkBigRequests() {
        return big_requests;
    }
    /** @brief Allows for extension flag access by extension name string. */
    std::unordered_map< std::string_view, _MembFuncs > _funcs_by_ext_name {
        { "BIG-REQUESTS",
          _MembFuncs{ &Extensions::_toggleBigRequests, &Extensions::_checkBigRequests } }
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
        auto it { _funcs_by_ext_name.find( name ) };
        if ( it != _funcs_by_ext_name.end() ) {
            const _ToggleFuncT toggle_func { it->second.toggle_func };
            (this->*toggle_func)( _Toggle::On );
        }
    }
    /**
     * @brief Check by name if extension is active.
     * @param name extension name
     * @return whether extension is active
     */
    inline bool
    active( const std::string_view& name ) {
        auto it { _funcs_by_ext_name.find( name ) };
        if ( it != _funcs_by_ext_name.end() ) {
            const _CheckFuncT check_func { it->second.check_func };
            return (this->*check_func)();
        }
        return false;
    }
    /**
     * @brief Deactivate an extension by name.
     * @param name extension name
     */
    inline void
    deactivate( const std::string_view& name ) {
        auto it { _funcs_by_ext_name.find( name ) };
        if ( it != _funcs_by_ext_name.end() ) {
            const _ToggleFuncT toggle_func { it->second.toggle_func };
            (this->*toggle_func)( _Toggle::Off );
        }
    }
};


#endif  // EXTENSIONS_HPP
