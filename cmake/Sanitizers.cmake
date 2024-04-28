function(
  dev_enable_sanitizers
  project_name
  ENABLE_ASAN
  ENABLE_LSAN
  ENABLE_UBSAN
  ENABLE_TSAN
  ENABLE_MSAN
)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(SANITIZERS "")

    if(${ENABLE_ASAN})
      list(APPEND SANITIZERS "address")
    endif()

    if(${ENABLE_LSAN})
      list(APPEND SANITIZERS "leak")
    endif()

    if(${ENABLE_UBSAN})
      list(APPEND SANITIZERS "undefined")
    endif()

    if(${ENABLE_TSAN})
      if("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
        message(WARNING " * tsan does not work with asan and lsan enabled")
      else()
        list(APPEND SANITIZERS "thread")
      endif()
    endif()

    if(${ENABLE_MSAN} AND CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
      message(WARNING " * msan requires all code to be msan-instrumented otherwise it reports false-positives")

      if("address" IN_LIST SANITIZERS
        OR "thread" IN_LIST SANITIZERS
        OR "leak" IN_LIST SANITIZERS)
        message(WARNING " * msan does not work with asan, tsan or lsan enabled")
      else()
        list(APPEND SANITIZERS "memory")
      endif()
    endif()
  elseif(MSVC)
    if(${ENABLE_ASAN})
      list(APPEND SANITIZERS "address")
    endif()

    if(${ENABLE_LSAN} OR ${ENABLE_UBSAN} OR ${ENABLE_TSAN} OR ${ENABLE_MSAN})
      message(WARNING " * MSVC only supports asan")
    endif()
  endif()

  list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

  if(LIST_OF_SANITIZERS)
    if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
      if(NOT MSVC)
        target_compile_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
        target_link_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
      else()
        string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" _VS_INSTALL_DIR_INDEX)

        if("${_VS_INSTALL_DIR_INDEX}" STREQUAL "-1")
          message(
            SEND_ERROR
            " * using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project."
          )
        endif()

        target_compile_options(${project_name} INTERFACE /fsanitize=${LIST_OF_SANITIZERS} /Zi /INCREMENTAL:NO)
        target_compile_definitions(${project_name} INTERFACE _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION)
        target_link_options(${project_name} INTERFACE /INCREMENTAL:NO)
      endif()
    endif()
  endif()
endfunction()
