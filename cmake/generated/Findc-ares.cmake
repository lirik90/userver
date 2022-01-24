# AUTOGENERATED, DON'T CHANGE THIS FILE!

if (NOT c-ares_FIND_VERSION OR c-ares_FIND_VERSION VERSION_LESS 1.16.0)
    set(c-ares_FIND_VERSION 1.16.0)
endif()

if (TARGET c-ares)
  if (NOT c-ares_FIND_VERSION)
      set(c-ares_FOUND ON)
      return()
  endif()

  if (c-ares_VERSION)
      if (c-ares_FIND_VERSION VERSION_LESS_EQUAL c-ares_VERSION)
          set(c-ares_FOUND ON)
          return()
      else()
          message(FATAL_ERROR
              "Already using version ${c-ares_VERSION} "
              "of c-ares when version ${c-ares_FIND_VERSION} "
              "was requested."
          )
      endif()
  endif()
endif()

set(FULL_ERROR_MESSAGE "Could not find `c-ares` package.\n\tDebian: sudo apt update && sudo apt install libc-ares-dev\n\tMacOS: brew install c-ares")


include(FindPackageHandleStandardArgs)

find_library(c-ares_LIBRARIES_cares
  NAMES cares
)
list(APPEND c-ares_LIBRARIES ${c-ares_LIBRARIES_cares})

find_path(c-ares_INCLUDE_DIRS_ares_h
  NAMES ares.h
)
list(APPEND c-ares_INCLUDE_DIRS ${c-ares_INCLUDE_DIRS_ares_h})



if (c-ares_VERSION)
  set(c-ares_VERSION ${c-ares_VERSION})
endif()

if (c-ares_FIND_VERSION AND NOT c-ares_VERSION)
if (UNIX AND NOT APPLE)
  find_program(DPKG_QUERY_BIN dpkg-query)
  if (DPKG_QUERY_BIN)
    execute_process(
      COMMAND dpkg-query --showformat=\${Version} --show libc-ares-dev
      OUTPUT_VARIABLE c-ares_version_output
      ERROR_VARIABLE c-ares_version_error
      RESULT_VARIABLE c-ares_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (c-ares_version_result EQUAL 0)
      set(c-ares_VERSION ${c-ares_version_output})
      message(STATUS "Installed version libc-ares-dev: ${c-ares_VERSION}")
    endif(c-ares_version_result EQUAL 0)
  endif(DPKG_QUERY_BIN)
endif(UNIX AND NOT APPLE)
 
if (APPLE)
  find_program(BREW_BIN brew)
  if (BREW_BIN)
    execute_process(
      COMMAND brew list --versions c-ares
      OUTPUT_VARIABLE c-ares_version_output
      ERROR_VARIABLE c-ares_version_error
      RESULT_VARIABLE c-ares_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (c-ares_version_result EQUAL 0)
      if (c-ares_version_output MATCHES "^(.*) (.*)$")
        set(c-ares_VERSION ${CMAKE_MATCH_2})
        message(STATUS "Installed version c-ares: ${c-ares_VERSION}")
      else()
        set(c-ares_VERSION "NOT_FOUND")
      endif()
    else()
      message(WARNING "Failed execute brew: ${c-ares_version_error}")
    endif()
  endif()
endif()
 
endif (c-ares_FIND_VERSION AND NOT c-ares_VERSION)

 
find_package_handle_standard_args(
  c-ares
    REQUIRED_VARS
      c-ares_LIBRARIES
      c-ares_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  c-ares_LIBRARIES
  c-ares_INCLUDE_DIRS
  
)

if (NOT c-ares_FOUND)
  if (c-ares_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}. Required version is at least ${c-ares_FIND_VERSION}")
  endif()

  return()
endif()

if (c-ares_FIND_VERSION)
  if (c-ares_VERSION VERSION_LESS c-ares_FIND_VERSION)
      message(STATUS
          "Version of c-ares is '${c-ares_VERSION}'. "
          "Required version is at least '${c-ares_FIND_VERSION}'. "
          "Ignoring found c-ares."
      )
      set(c-ares_FOUND OFF)
      return()
  endif()
endif()

 
if (NOT TARGET c-ares)
  add_library(c-ares INTERFACE IMPORTED GLOBAL)

  target_include_directories(c-ares INTERFACE ${c-ares_INCLUDE_DIRS})
  target_link_libraries(c-ares INTERFACE ${c-ares_LIBRARIES})
  
  # Target c-ares is created
endif()

if (c-ares_VERSION)
  set(c-ares_VERSION "${c-ares_VERSION}" CACHE STRING "Version of the c-ares")
endif()
