macro(dev_enable_cppcheck WARNINGS_AS_ERRORS CPPCHECK_OPTIONS)
  find_program(CPPCHECK cppcheck)

  if(CPPCHECK)
    if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
      set(CPPCHECK_TEMPLATE "vs")
    else()
      set(CPPCHECK_TEMPLATE "gcc")
    endif()

    if("${CPPCHECK_OPTIONS}" STREQUAL "")
      set(SUPPRESS_DIR "*:${CMAKE_CURRENT_BINARY_DIR}/_deps/*.h")
      message(STATUS " * cppcheck_options suppress: ${SUPPRESS_DIR}")
      set(
        CMAKE_CXX_CPPCHECK
        --template=${CPPCHECK_TEMPLATE}
        --enable=style,performance,warning,portability
        --inline-suppr
        --suppress=cppcheckError
        --suppress=internalAstError
        --suppress=unmatchedSuppression
        --suppress=passedByValue
        --suppress=syntaxError
        --suppress=preprocessorErrorDirective
        --inconclusive
        --suppress=${SUPPRESS_DIR}
      )
    else()
      set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --template=${CPPCHECK_TEMPLATE} ${CPPCHECK_OPTIONS})
    endif()

    if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
      set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} -std=c++${CMAKE_CXX_STANDARD})
    endif()

    if(${WARNINGS_AS_ERRORS})
      list(APPEND CMAKE_CXX_CPPCHECK --error-exitcode=2)
    endif()
  else()
    message(STATUS " * cppcheck requested but not found")
  endif()
endmacro()

macro(dev_enable_clang_tidy target WARNINGS_AS_ERRORS)
  find_program(CLANGTIDY clang-tidy)

  if(CLANGTIDY)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
      get_target_property(_TARGET_PCH ${target} INTERFACE_PRECOMPILE_HEADERS)

      if(NOT("${_TARGET_PCH}" STREQUAL "TARGET_PCH-NOTFOUND"))
        message(SEND_ERROR " * clang-tidy cannot be enabled with non-clang compiler and pch")
      endif()
    endif()

    set(
      CLANG_TIDY_OPTIONS
      ${CLANGTIDY}
      -extra-arg=-Wno-unknown-warning-option
      -extra-arg=-Wno-ignored-optimization-argument
      -extra-arg=-Wno-unused-command-line-argument
      -p
    )

    set(_TARGET_LANG "")
    get_target_property(_TARGET_LANG ${target} LINKER_LANGUAGE)

    if("${_TARGET_LANG}" STREQUAL "CXX" AND NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
      if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
      else()
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
      endif()
    elseif("${TARGET_LANG}" STREQUAL "C" AND NOT "${CMAKE_C_STANDARD}" STREQUAL "")
      if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c${CMAKE_C_STANDARD})
      else()
        set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c${CMAKE_C_STANDARD})
      endif()
    endif()

    if(${WARNINGS_AS_ERRORS})
      list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
    endif()

    message(STATUS " * setting clang-tidy globally...")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
  else()
    message(STATUS " * clang-tidy requested but not found")
  endif()
endmacro()
