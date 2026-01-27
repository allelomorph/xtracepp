cmake_minimum_required(VERSION 3.14)

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(src OBJECT
  bufferHexDump.cpp
  errors.cpp
  main.cpp
  Connection.cpp
  DisplayInfo.cpp
  ProxyX11Server.cpp
  ProxyX11Server_prequeue_clients.cpp
  Settings.cpp
  SocketBuffer.cpp
  X11ProtocolParser.cpp
  X11ProtocolParser__formatVariable.cpp
  X11ProtocolParser__logConnectionSetup.cpp
  X11ProtocolParser__parseRequest.cpp
  X11ProtocolParser__parseReply.cpp
  X11ProtocolParser__parseEvent.cpp
  X11ProtocolParser__parseError.cpp
  X11ProtocolParser__parseListMember.cpp
  X11ProtocolParser__core_requests.cpp
  X11ProtocolParser__core_events.cpp
  X11ProtocolParser__core_errors.cpp
)

set_strict_compile_options(src)
set_target_properties(src PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

set(XTRACEPP_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(src PRIVATE
  ${XTRACEPP_INCLUDE_PATH}
)

target_link_libraries(src PUBLIC
  fmt
)

add_subdirectory(extensions)
