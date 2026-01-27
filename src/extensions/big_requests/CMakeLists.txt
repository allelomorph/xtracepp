cmake_minimum_required(VERSION 3.14)

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

add_library(big_requests OBJECT
  X11ProtocolParser__parseRequest.cpp
  X11ProtocolParser__parseReply.cpp
)

set_strict_compile_options(big_requests)
set_target_properties(big_requests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_include_directories(big_requests PRIVATE
  ${XTRACEPP_INCLUDE_PATH}
)

target_link_libraries(big_requests PUBLIC
  fmt
)
