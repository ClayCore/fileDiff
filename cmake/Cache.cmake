function(dev_enable_cache)
  set(CACHE_OPTION "ccache" CACHE STRING "Compiler cache to be used")
  set(CACHE_OPTION_VALUES "ccache" "sccache")

  set_property(CACHE CACHE_OPTION PROPERTY STRINGS ${CACHE_OPTION_VALUES})
  list(FIND CACHE_OPTION_VALUES ${CACHE_OPTION} CACHE_OPTION_INDEX)

  if(${CACHE_OPTION_INDEX} EQUAL -1)
    message(STATUS " * using custom compiler cache: '${CACHE_OPTION}'\nsupported entries: ${CACHE_OPTION_VALUES}")
  endif()

  find_program(CACHE_BINARY NAMES ${CACHE_OPTION_VALUES})

  if(CACHE_BINARY)
    message(STATUS " * ${CACHE_BINARY} found and enabled")
    set(
      CMAKE_CXX_COMPILER_LAUNCHER
      ${CACHE_BINARY}
      CACHE FILEPATH "CXX compiler cache used"
    )
    set(
      CMAKE_C_COMPILER_LAUNCHER
      ${CACHE_BINARY}
      CACHE FILEPATH "C compiler cache used"
    )
  else()
    message(WARNING " * ${CACHE_OPTION} enabled but not found. Dismissing")
  endif()
endfunction()
