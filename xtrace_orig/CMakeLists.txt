# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

project(xtracepp
  DESCRIPTION "Proxy between X11 server and clients, logging comms and \
selectively altering packet data based on user options. Rebuild in C++ of \
xtrace C source by Bernhard Link et al.."
  LANGUAGES C CXX
)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/config.h)
  execute_process(
    COMMAND ./configure
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    #OUTPUT_QUIET
    COMMAND_ERROR_IS_FATAL ANY
  )
endif()

add_executable(_xtrace
  atoms.c
  copyauth.c
  main.c
  parse.c
  stringlist.c
  translate.c
  x11client.c
  x11common.c
  x11server.c
)

target_include_directories(_xtrace PRIVATE
  ${PROJECT_SOURCE_DIR}
)

# allows use of .proto Protobuf files
# no need for FindProtobuf.cmake
target_compile_definitions(_xtrace PUBLIC
  PKGDATADIR="${PROJECT_SOURCE_DIR}"
)
