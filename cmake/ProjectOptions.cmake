include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(dev_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(dev_options_setup)
  option(dev_ENABLE_HARDENING "Enable hardening" ON)
  option(dev_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    dev_ENABLE_GLOBAL_HARDENING
    "Push hardening options to built dependencies"
    ON
    dev_ENABLE_HARDENING
    OFF
  )

  dev_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR dev_PACKAGING_MAINTAINER_MODE)
    option(dev_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(dev_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(dev_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(dev_ENABLE_ASAN "Enable address sanitizer" OFF)
    option(dev_ENABLE_LSAN "Enable leak sanitizer" OFF)
    option(dev_ENABLE_UBSAN "Enable undefined sanitizer" OFF)
    option(dev_ENABLE_TSAN "Enable thread sanitizer" OFF)
    option(dev_ENABLE_MSAN "Enable memory sanitizer" OFF)
    option(dev_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(dev_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(dev_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(dev_ENABLE_PCH "Enable precompiled headers" OFF)
    option(dev_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(dev_ENABLE_IPO "Enable IPO/LTO" ON)
    option(dev_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(dev_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(dev_ENABLE_ASAN "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(dev_ENABLE_LSAN "Enable leak sanitizer" OFF)
    option(dev_ENABLE_UBSAN "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(dev_ENABLE_TSAN "Enable thread sanitizer" OFF)
    option(dev_ENABLE_MSAN "Enable memory sanitizer" OFF)
    option(dev_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(dev_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(dev_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(dev_ENABLE_PCH "Enable precompiled headers" OFF)
    option(dev_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      dev_ENABLE_IPO
      dev_WARNINGS_AS_ERRORS
      dev_ENABLE_USER_LINKER
      dev_ENABLE_ASAN
      dev_ENABLE_LSAN
      dev_ENABLE_UBSAN
      dev_ENABLE_TSAN
      dev_ENABLE_MSAN
      dev_ENABLE_UNITY_BUILD
      dev_ENABLE_CLANG_TIDY
      dev_ENABLE_CPPCHECK
      dev_ENABLE_COVERAGE
      dev_ENABLE_PCH
      dev_ENABLE_CACHE
    )
  endif()

  dev_check_libfuzzer_support(LIBFUZZER_SUPPORTED)

  if(LIBFUZZER_SUPPORTED AND(dev_ENABLE_ASAN OR dev_ENABLE_TSAN OR dev_ENABLE_UBSAN))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(dev_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})
endmacro()

macro(dev_options_global)
  if(dev_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    dev_enable_ipo()
  endif()

  dev_supports_sanitizers()

  if(dev_ENABLE_HARDENING AND dev_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)

    if(
      NOT SUPPORTS_UBSAN
      OR dev_ENABLE_UBSAN
      OR dev_ENABLE_ASAN
      OR dev_ENABLE_TSAN
      OR dev_ENABLE_LSAN
    )
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
  endif()
endmacro()

macro(dev_options_local)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/GlobalProjectSettings.cmake)
  endif()

  add_library(dev_warnings INTERFACE)
  add_library(dev_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  dev_set_project_warnings(
    dev_warnings
    ${dev_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    ""
  )

  if(dev_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    dev_configure_linker(dev_options)
  endif()

  include(cmake/Sanitizers.cmake)
  dev_enable_sanitizers(
    dev_options
    ${dev_ENABLE_ASAN}
    ${dev_ENABLE_LSAN}
    ${dev_ENABLE_UBSAN}
    ${dev_ENABLE_TSAN}
    ${dev_ENABLE_MSAN}
  )

  set_target_properties(dev_options PROPERTIES UNITY_BUILD ${dev_ENABLE_UNITY_BUILD})

  if(dev_ENABLE_PCH)
    target_precompile_headers(
      dev_options
      INTERFACE
      <vector>
      <string>
      <utility>
    )
  endif()

  if(dev_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    dev_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)

  if(dev_ENABLE_CLANG_TIDY)
    dev_enable_clang_tidy(dev_options ${dev_WARNINGS_AS_ERRORS})
  endif()

  if(dev_ENABLE_CPPCHECK)
    dev_enable_cppcheck(${dev_WARNINGS_AS_ERRORS} "")
  endif()

  if(dev_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    dev_enable_coverage(dev_options)
  endif()

  if(dev_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)

    if(LINKER_FATAL_WARNINGS)
      # CLANG_TIDY_OPTIONS_DRIVER_MODE
    endif()
  endif()

  if(dev_ENABLE_HARDENING AND NOT dev_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)

    if(
      NOT SUPPORTS_UBSAN
      OR dev_ENABLE_UBSAN
      OR dev_ENABLE_ASAN
      OR dev_ENABLE_TSAN
      OR dev_ENABLE_LSAN
    )
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()

    dev_enable_hardening(dev_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()
