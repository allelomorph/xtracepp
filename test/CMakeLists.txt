# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

find_package(X11 REQUIRED)
if(NOT (X11_xcb_LIB AND X11_xcb_INCLUDE_PATH))
  message(FATAL_ERROR
    "X11_xcb_LIB or X11_xcb_INCLUDE_PATH not defined by FindX11.cmake")
endif()

#[[
add_executable(001_CreateWindow_test
  001_CreateWindow_test.cpp
)
target_include_directories(001_CreateWindow_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
)
target_link_libraries(001_CreateWindow_test PUBLIC
  ${X11_xcb_LIB}
)
]]

add_executable(request_test
  request_test.cpp
)
target_include_directories(request_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
  ${PROJECT_SOURCE_DIR}/src/include
)
target_link_libraries(request_test PUBLIC
  ${X11_xcb_LIB}
)

add_executable(ServerRefusal_test
  ServerRefusal_test.cpp
)
target_include_directories(ServerRefusal_test PRIVATE
  ${X11_xcb_INCLUDE_PATH}
)
target_link_libraries(ServerRefusal_test PUBLIC
  ${X11_xcb_LIB}
)
