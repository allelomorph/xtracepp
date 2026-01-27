cmake_minimum_required(VERSION 3.14)

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

add_library(extensions OBJECT
  X11ProtocolParser__extensions.cpp
  X11ProtocolParser__parseRequest.cpp
)

set_strict_compile_options(extensions)
set_target_properties(extensions PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_include_directories(extensions PRIVATE
  ${XTRACEPP_INCLUDE_PATH}
)

target_link_libraries(extensions PUBLIC
  fmt
)

add_subdirectory(big_requests)
