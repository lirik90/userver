# AUTOGENERATED, DON'T CHANGE THIS FILE!


if (TARGET iostreams)
  if (NOT iostreams_FIND_VERSION)
      set(iostreams_FOUND ON)
      return()
  endif()

  if (iostreams_VERSION)
      if (iostreams_FIND_VERSION VERSION_LESS_EQUAL iostreams_VERSION)
          set(iostreams_FOUND ON)
          return()
      else()
          message(FATAL_ERROR
              "Already using version ${iostreams_VERSION} "
              "of iostreams when version ${iostreams_FIND_VERSION} "
              "was requested."
          )
      endif()
  endif()
endif()

set(FULL_ERROR_MESSAGE "Could not find `iostreams` package.\n\tDebian: sudo apt update && sudo apt install libboost-iostreams-dev\n\tMacOS: brew install boost")


include(FindPackageHandleStandardArgs)

find_library(iostreams_LIBRARIES_boost_iostreams
  NAMES boost_iostreams
)
list(APPEND iostreams_LIBRARIES ${iostreams_LIBRARIES_boost_iostreams})

find_path(iostreams_INCLUDE_DIRS_boost_iostreams_seek_hpp
  NAMES boost/iostreams/seek.hpp
)
list(APPEND iostreams_INCLUDE_DIRS ${iostreams_INCLUDE_DIRS_boost_iostreams_seek_hpp})



if (iostreams_VERSION)
  set(iostreams_VERSION ${iostreams_VERSION})
endif()

if (iostreams_FIND_VERSION AND NOT iostreams_VERSION)
if (UNIX AND NOT APPLE)
  find_program(DPKG_QUERY_BIN dpkg-query)
  if (DPKG_QUERY_BIN)
    execute_process(
      COMMAND dpkg-query --showformat=\${Version} --show libboost-iostreams-dev
      OUTPUT_VARIABLE iostreams_version_output
      ERROR_VARIABLE iostreams_version_error
      RESULT_VARIABLE iostreams_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (iostreams_version_result EQUAL 0)
      set(iostreams_VERSION ${iostreams_version_output})
      message(STATUS "Installed version libboost-iostreams-dev: ${iostreams_VERSION}")
    endif(iostreams_version_result EQUAL 0)
  endif(DPKG_QUERY_BIN)
endif(UNIX AND NOT APPLE)
 
if (APPLE)
  find_program(BREW_BIN brew)
  if (BREW_BIN)
    execute_process(
      COMMAND brew list --versions boost
      OUTPUT_VARIABLE iostreams_version_output
      ERROR_VARIABLE iostreams_version_error
      RESULT_VARIABLE iostreams_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (iostreams_version_result EQUAL 0)
      if (iostreams_version_output MATCHES "^(.*) (.*)$")
        set(iostreams_VERSION ${CMAKE_MATCH_2})
        message(STATUS "Installed version boost: ${iostreams_VERSION}")
      else()
        set(iostreams_VERSION "NOT_FOUND")
      endif()
    else()
      message(WARNING "Failed execute brew: ${iostreams_version_error}")
    endif()
  endif()
endif()
 
endif (iostreams_FIND_VERSION AND NOT iostreams_VERSION)

 
find_package_handle_standard_args(
  iostreams
    REQUIRED_VARS
      iostreams_LIBRARIES
      iostreams_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  iostreams_LIBRARIES
  iostreams_INCLUDE_DIRS
  
)

if (NOT iostreams_FOUND)
  if (iostreams_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}. Required version is at least ${iostreams_FIND_VERSION}")
  endif()

  return()
endif()

if (iostreams_FIND_VERSION)
  if (iostreams_VERSION VERSION_LESS iostreams_FIND_VERSION)
      message(STATUS
          "Version of iostreams is '${iostreams_VERSION}'. "
          "Required version is at least '${iostreams_FIND_VERSION}'. "
          "Ignoring found iostreams."
      )
      set(iostreams_FOUND OFF)
      return()
  endif()
endif()

 
if (NOT TARGET iostreams)
  add_library(iostreams INTERFACE IMPORTED GLOBAL)

  if (TARGET Boost::iostreams)
    target_link_libraries(iostreams INTERFACE Boost::iostreams)
  endif()
  target_include_directories(iostreams INTERFACE ${iostreams_INCLUDE_DIRS})
  target_link_libraries(iostreams INTERFACE ${iostreams_LIBRARIES})
  
  # Target iostreams is created
endif()

if (iostreams_VERSION)
  set(iostreams_VERSION "${iostreams_VERSION}" CACHE STRING "Version of the iostreams")
endif()
