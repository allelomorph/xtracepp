# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

add_executable(xtracepp
  errors.cpp
  main.cpp
  Connection.cpp
  DisplayInfo.cpp
  ProxyX11Server.cpp
  ProxyX11Server_prequeue_clients.cpp
  Settings.cpp
  SocketBuffer.cpp
  X11ProtocolParser.cpp
  X11ProtocolParser__logConnInitiation.cpp
  X11ProtocolParser__logConnResponse.cpp
  X11ProtocolParser__logRequest.cpp
  X11ProtocolParser__logError.cpp
  X11ProtocolParser__logEvent.cpp
  X11ProtocolParser__logReply.cpp
  X11ProtocolParser__formatCommonType.cpp
#  X11ProtocolParser__parseProtocolType.cpp
  bufferHexDump.cpp
)

target_include_directories(xtracepp PRIVATE
  include
)

target_link_libraries(xtracepp PUBLIC
  fmt
)
