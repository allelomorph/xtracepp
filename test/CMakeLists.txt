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

add_executable(request_test
  request_test.cpp
)
set_strict_compile_options(request_test)
set_target_properties(request_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(request_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(request_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(reply_test
  reply_test.cpp
)
set_strict_compile_options(reply_test)
set_target_properties(reply_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(reply_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(reply_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(event_test_sendevent
  event_test_sendevent.cpp
)
set_strict_compile_options(event_test_sendevent)
set_target_properties(event_test_sendevent PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(event_test_sendevent PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(event_test_sendevent PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(error_test
  error_test.cpp
)
set_strict_compile_options(error_test)
set_target_properties(error_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(error_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(error_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)
