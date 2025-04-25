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
)

target_include_directories(xtracepp PRIVATE
  include
)
