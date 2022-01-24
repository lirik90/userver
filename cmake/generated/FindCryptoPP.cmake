# AUTOGENERATED, DON'T CHANGE THIS FILE!


if (TARGET CryptoPP)
  if (NOT CryptoPP_FIND_VERSION)
      set(CryptoPP_FOUND ON)
      return()
  endif()

  if (CryptoPP_VERSION)
      if (CryptoPP_FIND_VERSION VERSION_LESS_EQUAL CryptoPP_VERSION)
          set(CryptoPP_FOUND ON)
          return()
      else()
          message(FATAL_ERROR
              "Already using version ${CryptoPP_VERSION} "
              "of CryptoPP when version ${CryptoPP_FIND_VERSION} "
              "was requested."
          )
      endif()
  endif()
endif()

set(FULL_ERROR_MESSAGE "Could not find `CryptoPP` package.\n\tDebian: sudo apt update && sudo apt install libcrypto++-dev\n\tMacOS: brew install cryptopp")


include(FindPackageHandleStandardArgs)

find_library(CryptoPP_LIBRARIES_cryptlib_cryptopp
  NAMES cryptlib cryptopp
  PATH_SUFFIXES lib
)
list(APPEND CryptoPP_LIBRARIES ${CryptoPP_LIBRARIES_cryptlib_cryptopp})

find_path(CryptoPP_INCLUDE_DIRS_cryptopp_cryptlib_h
  NAMES cryptopp/cryptlib.h
  PATH_SUFFIXES include
)
list(APPEND CryptoPP_INCLUDE_DIRS ${CryptoPP_INCLUDE_DIRS_cryptopp_cryptlib_h})



if (CryptoPP_VERSION)
  set(CryptoPP_VERSION ${CryptoPP_VERSION})
endif()

if (CryptoPP_FIND_VERSION AND NOT CryptoPP_VERSION)
if (UNIX AND NOT APPLE)
  find_program(DPKG_QUERY_BIN dpkg-query)
  if (DPKG_QUERY_BIN)
    execute_process(
      COMMAND dpkg-query --showformat=\${Version} --show libcrypto++-dev
      OUTPUT_VARIABLE CryptoPP_version_output
      ERROR_VARIABLE CryptoPP_version_error
      RESULT_VARIABLE CryptoPP_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (CryptoPP_version_result EQUAL 0)
      set(CryptoPP_VERSION ${CryptoPP_version_output})
      message(STATUS "Installed version libcrypto++-dev: ${CryptoPP_VERSION}")
    endif(CryptoPP_version_result EQUAL 0)
  endif(DPKG_QUERY_BIN)
endif(UNIX AND NOT APPLE)
 
if (APPLE)
  find_program(BREW_BIN brew)
  if (BREW_BIN)
    execute_process(
      COMMAND brew list --versions cryptopp
      OUTPUT_VARIABLE CryptoPP_version_output
      ERROR_VARIABLE CryptoPP_version_error
      RESULT_VARIABLE CryptoPP_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (CryptoPP_version_result EQUAL 0)
      if (CryptoPP_version_output MATCHES "^(.*) (.*)$")
        set(CryptoPP_VERSION ${CMAKE_MATCH_2})
        message(STATUS "Installed version cryptopp: ${CryptoPP_VERSION}")
      else()
        set(CryptoPP_VERSION "NOT_FOUND")
      endif()
    else()
      message(WARNING "Failed execute brew: ${CryptoPP_version_error}")
    endif()
  endif()
endif()
 
endif (CryptoPP_FIND_VERSION AND NOT CryptoPP_VERSION)

 
find_package_handle_standard_args(
  CryptoPP
    REQUIRED_VARS
      CryptoPP_LIBRARIES
      CryptoPP_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  CryptoPP_LIBRARIES
  CryptoPP_INCLUDE_DIRS
  
)

if (NOT CryptoPP_FOUND)
  if (CryptoPP_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}. Required version is at least ${CryptoPP_FIND_VERSION}")
  endif()

  return()
endif()

if (CryptoPP_FIND_VERSION)
  if (CryptoPP_VERSION VERSION_LESS CryptoPP_FIND_VERSION)
      message(STATUS
          "Version of CryptoPP is '${CryptoPP_VERSION}'. "
          "Required version is at least '${CryptoPP_FIND_VERSION}'. "
          "Ignoring found CryptoPP."
      )
      set(CryptoPP_FOUND OFF)
      return()
  endif()
endif()

 
if (NOT TARGET CryptoPP)
  add_library(CryptoPP INTERFACE IMPORTED GLOBAL)

  target_include_directories(CryptoPP INTERFACE ${CryptoPP_INCLUDE_DIRS})
  target_link_libraries(CryptoPP INTERFACE ${CryptoPP_LIBRARIES})
  
  # Target CryptoPP is created
endif()

if (CryptoPP_VERSION)
  set(CryptoPP_VERSION "${CryptoPP_VERSION}" CACHE STRING "Version of the CryptoPP")
endif()
