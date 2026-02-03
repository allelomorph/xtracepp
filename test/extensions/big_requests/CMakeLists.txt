# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

find_package(X11 REQUIRED)
if(NOT (X11_xcb_LIB AND X11_xcb_INCLUDE_PATH))
  message(FATAL_ERROR
    "X11_xcb_LIB or X11_xcb_INCLUDE_PATH not defined by FindX11.cmake")
endif()

add_executable(bigreq_request_test
  request_test.cpp
)
set_strict_compile_options(bigreq_request_test)
set_target_properties(bigreq_request_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(bigreq_request_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(bigreq_request_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)
