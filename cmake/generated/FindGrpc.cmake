# AUTOGENERATED, DON'T CHANGE THIS FILE!


if (TARGET Grpc)
  if (NOT Grpc_FIND_VERSION)
      set(Grpc_FOUND ON)
      return()
  endif()

  if (Grpc_VERSION)
      if (Grpc_FIND_VERSION VERSION_LESS_EQUAL Grpc_VERSION)
          set(Grpc_FOUND ON)
          return()
      else()
          message(FATAL_ERROR
              "Already using version ${Grpc_VERSION} "
              "of Grpc when version ${Grpc_FIND_VERSION} "
              "was requested."
          )
      endif()
  endif()
endif()

set(FULL_ERROR_MESSAGE "Could not find `Grpc` package.\n\tDebian: sudo apt update && sudo apt install libyandex-taxi-grpc-dev libyandex-taxi-grpc7 libyandex-taxi-grpc++-dev libyandex-taxi-grpc++1 yandex-taxi-protobuf-compiler-grpc\n\tMacOS: brew install grpc")


include(FindPackageHandleStandardArgs)

find_library(Grpc_LIBRARIES_grpc
  NAMES grpc
)
list(APPEND Grpc_LIBRARIES ${Grpc_LIBRARIES_grpc})
find_library(Grpc_LIBRARIES_grpc++
  NAMES grpc++
)
list(APPEND Grpc_LIBRARIES ${Grpc_LIBRARIES_grpc++})
find_library(Grpc_LIBRARIES_gpr
  NAMES gpr
)
list(APPEND Grpc_LIBRARIES ${Grpc_LIBRARIES_gpr})
find_library(Grpc_LIBRARIES_yandex-taxi-cares_cares
  NAMES yandex-taxi-cares cares
)
list(APPEND Grpc_LIBRARIES ${Grpc_LIBRARIES_yandex-taxi-cares_cares})
find_library(Grpc_LIBRARIES_address_sorting
  NAMES address_sorting
)
list(APPEND Grpc_LIBRARIES ${Grpc_LIBRARIES_address_sorting})

find_path(Grpc_INCLUDE_DIRS_grpc_grpc_h
  NAMES grpc/grpc.h
)
list(APPEND Grpc_INCLUDE_DIRS ${Grpc_INCLUDE_DIRS_grpc_grpc_h})
find_path(Grpc_INCLUDE_DIRS_google_protobuf_port_def_inc
  NAMES google/protobuf/port_def.inc
  PATH_SUFFIXES yandex-taxi-proto3
)
list(APPEND Grpc_INCLUDE_DIRS ${Grpc_INCLUDE_DIRS_google_protobuf_port_def_inc})



if (Grpc_VERSION)
  set(Grpc_VERSION ${Grpc_VERSION})
endif()

if (Grpc_FIND_VERSION AND NOT Grpc_VERSION)
if (UNIX AND NOT APPLE)
  find_program(DPKG_QUERY_BIN dpkg-query)
  if (DPKG_QUERY_BIN)
    execute_process(
      COMMAND dpkg-query --showformat=\${Version} --show libyandex-taxi-grpc-dev
      OUTPUT_VARIABLE Grpc_version_output
      ERROR_VARIABLE Grpc_version_error
      RESULT_VARIABLE Grpc_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (Grpc_version_result EQUAL 0)
      set(Grpc_VERSION ${Grpc_version_output})
      message(STATUS "Installed version libyandex-taxi-grpc-dev: ${Grpc_VERSION}")
    endif(Grpc_version_result EQUAL 0)
  endif(DPKG_QUERY_BIN)
endif(UNIX AND NOT APPLE)
 
if (APPLE)
  find_program(BREW_BIN brew)
  if (BREW_BIN)
    execute_process(
      COMMAND brew list --versions grpc
      OUTPUT_VARIABLE Grpc_version_output
      ERROR_VARIABLE Grpc_version_error
      RESULT_VARIABLE Grpc_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (Grpc_version_result EQUAL 0)
      if (Grpc_version_output MATCHES "^(.*) (.*)$")
        set(Grpc_VERSION ${CMAKE_MATCH_2})
        message(STATUS "Installed version grpc: ${Grpc_VERSION}")
      else()
        set(Grpc_VERSION "NOT_FOUND")
      endif()
    else()
      message(WARNING "Failed execute brew: ${Grpc_version_error}")
    endif()
  endif()
endif()
 
endif (Grpc_FIND_VERSION AND NOT Grpc_VERSION)

 
find_package_handle_standard_args(
  Grpc
    REQUIRED_VARS
      Grpc_LIBRARIES
      Grpc_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  Grpc_LIBRARIES
  Grpc_INCLUDE_DIRS
  
)

if (NOT Grpc_FOUND)
  if (Grpc_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}. Required version is at least ${Grpc_FIND_VERSION}")
  endif()

  return()
endif()

if (Grpc_FIND_VERSION)
  if (Grpc_VERSION VERSION_LESS Grpc_FIND_VERSION)
      message(STATUS
          "Version of Grpc is '${Grpc_VERSION}'. "
          "Required version is at least '${Grpc_FIND_VERSION}'. "
          "Ignoring found Grpc."
      )
      set(Grpc_FOUND OFF)
      return()
  endif()
endif()

 
if (NOT TARGET Grpc)
  add_library(Grpc INTERFACE IMPORTED GLOBAL)

  target_include_directories(Grpc INTERFACE ${Grpc_INCLUDE_DIRS})
  target_link_libraries(Grpc INTERFACE ${Grpc_LIBRARIES})
  
  # Target Grpc is created
endif()

if (Grpc_VERSION)
  set(Grpc_VERSION "${Grpc_VERSION}" CACHE STRING "Version of the Grpc")
endif()
