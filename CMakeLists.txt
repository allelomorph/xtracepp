# newest features used: TBD
cmake_minimum_required(VERSION 3.14)

project(x11_extension_filter
  DESCRIPTION "Teardown of xtrace to provide selective filtering of x11 extensions"
  LANGUAGES C
)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/config.h)
  execute_process(
    COMMAND ./configure
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    #OUTPUT_QUIET
    COMMAND_ERROR_IS_FATAL ANY
  )
endif()

add_executable(x11_extension_filter
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
target_include_directories(x11_extension_filter PRIVATE
  ${PROJECT_SOURCE_DIR}
)

# allows use of .proto Protobuf files
# no need for FindProtobuf.cmake
target_compile_definitions(x11_extension_filter PUBLIC
  PKGDATADIR="${PROJECT_SOURCE_DIR}"
)
