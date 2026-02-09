# newest features used: PROJECT_IS_TOP_LEVEL
cmake_minimum_required(VERSION 3.21)

project(xtracepp
  DESCRIPTION "Proxy between X11 server and clients, logging comms and \
selectively altering packet data based on user options. Rebuild in C++ of \
xtrace C source by Bernhard Link et al.."
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Getcmake_utils)
include(PreventInSourceBuild)

include(Getfmt)

add_executable(xtracepp)
add_subdirectory(src)
target_link_libraries(xtracepp PUBLIC
  src
  extensions
  big_requests
)

if(XTRACEPP_BUILD_TESTING)
  add_subdirectory(test)
endif()

if(XTRACEPP_BUILD_DOCS AND PROJECT_IS_TOP_LEVEL)
  include(AddDoxygen)
  add_doxygen(${PROJECT_SOURCE_DIR}/src/
    EXCLUDE
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__formatVariable.cpp
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__logConnectionSetup.cpp
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__logError.cpp
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__logEvent.cpp
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__logReply.cpp
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__logRequest.cpp
    ${PROJECT_SOURCE_DIR}/src/X11ProtocolParser__parseListMember.cpp
    EXTRACT_PRIVATE
  )
endif()
