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

add_executable(core_request_test
  core_request_test.cpp
)
set_strict_compile_options(core_request_test)
set_target_properties(core_request_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(core_request_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(core_request_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(core_reply_test
  core_reply_test.cpp
)
set_strict_compile_options(core_reply_test)
set_target_properties(core_reply_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(core_reply_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(core_reply_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(core_event_test_sendevent
  core_event_test_sendevent.cpp
)
set_strict_compile_options(core_event_test_sendevent)
set_target_properties(core_event_test_sendevent PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(core_event_test_sendevent PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(core_event_test_sendevent PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(core_error_test
  core_error_test.cpp
)
set_strict_compile_options(core_error_test)
set_target_properties(core_error_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(core_error_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(core_error_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(systemtimeformat_test
  systemtimeformat_test.cpp
)
set_strict_compile_options(systemtimeformat_test)
set_target_properties(systemtimeformat_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(systemtimeformat_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(systemtimeformat_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_executable(prefetchatoms_test
  prefetchatoms_test.cpp
)
set_strict_compile_options(prefetchatoms_test)
set_target_properties(prefetchatoms_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)
target_include_directories(prefetchatoms_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(prefetchatoms_test PUBLIC
  ${X11_xcb_LIB}
  fmt
)

add_subdirectory(extensions)
