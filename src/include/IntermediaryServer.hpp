#ifndef INTERMEDIARYSERVER_HPP
#define INTERMEDIARYSERVER_HPP


#include <string>
#include <string_view>

#include <cstdint>      // SIZE_MAX

#include "Settings.hpp"

class IntermediaryServer {
private:
    static constexpr std::string_view _IN_DISPLAYNAME_ENV_VAR  { "FAKEDISPLAY" };
    static constexpr std::string_view _OUT_DISPLAYNAME_ENV_VAR { "DISPLAY" };
    static constexpr std::string_view _DEFAULT_IN_DISPLAYNAME  { ":9" };

    class _DisplayInfo {
    private:
        static constexpr int _UNSET { -1 };

    public:
        std::string name;
        std::string protocol;
        std::string hostname;
        int display { _UNSET };
        int screen  { _UNSET };
        int family  { _UNSET };

        //_DisplayInfo() = delete;
        _DisplayInfo() {}
        _DisplayInfo(const char* displayname);
    };

    _DisplayInfo _in_display;
    _DisplayInfo _out_display;

    void _initDisplays();

public:
    Settings settings;

    IntermediaryServer();

    void initDisplays();
    void __debugOutput();
};


#endif  // INTERMEDIARYSERVER_HPP
