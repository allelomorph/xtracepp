# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

project(xtracepp
  DESCRIPTION "Proxy between X11 server and clients, logging comms and \
selectively altering packet data based on user options. Rebuild in C++ of \
xtrace C source by Bernhard Link et al.."
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Getcmake_utils)

include(Getfmt)

add_subdirectory(src)

if(XTRACEPP_BUILD_TESTING)
  add_subdirectory(test)
endif()
