cmake_minimum_required(VERSION 3.14)

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

add_executable(xtracepp
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

set_strict_compile_options(xtracepp)
set_target_properties(xtracepp PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_include_directories(xtracepp PRIVATE
  include
)

target_link_libraries(xtracepp PUBLIC
  fmt
)
