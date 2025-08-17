# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

add_executable(xtracepp
  errors.cpp
  main.cpp
  Connection.cpp
  DisplayInfo.cpp
  ProxyX11Server.cpp
  Settings.cpp
  SocketBuffer.cpp
  X11ProtocolParser.cpp
  X11ProtocolParser__logClientRequest.cpp
  X11ProtocolParser__logServerError.cpp
  X11ProtocolParser__logServerEvent.cpp
  X11ProtocolParser__logServerReply.cpp
  X11ProtocolParser__format.cpp
)

target_include_directories(xtracepp PRIVATE
  include
)

target_link_libraries(xtracepp PUBLIC
  fmt
)
