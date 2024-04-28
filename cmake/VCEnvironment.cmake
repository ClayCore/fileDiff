include(cmake/Utilities.cmake)

macro(dev_detect_arch)
  string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" CMAKE_SYSTEM_PROCESSOR_LOWER)

  if(CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL x86 OR CMAKE_SYSTEM_PROCESSOR_LOWER MATCHES "^i[3456]86$")
    set(VCVARSALL_ARCH x86)
  elseif(
    CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL x64
    OR CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL x86_64
    OR CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL amd64
  )
    set(VCVARSALL_ARCH x64)
  elseif(CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL arm64 OR CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL aarch64)
    set(VCVARSALL_ARCH arm64)
  else()
    if(CMAKE_HOST_SYSTEM_PROCESSOR)
      set(VCVARSALL_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
    else()
      set(VCVARSALL_ARCH x64)
      message(WARNING " * unknown architecture \'${CMAKE_SYSTEM_PROCESSOR_LOWER}\' - using x64")
    endif()
  endif()
endmacro()

function(run_vcvarsall)
  message(STATUS " * trying to find and to run \'vcvarsall.bat\'...")

  if(MSVC AND "$ENV{VSCMD_VER}" STREQUAL "")
    get_filename_component(MSVC_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    find_file(
      VCVARSALL_FILE
      NAMES
      "vcvarsall.bat"
      PATHS
      "${MSVC_DIR}"
      "${MSVC_DIR}/.."
      "${MSVC_DIR}/../.."
      "${MSVC_DIR}/../../../../../../../.."
      "${MSVC_DIR}/../../../../../../.."
      PATH_SUFFIXES
      "VC/Auxiliary/Build"
      "Common7/Tools"
      "Tools"
    )

    if(EXISTS ${VCVARSALL_FILE})
      dev_detect_arch()

      set(VCVARSALL_CMD "\"${VCVARSALL_FILE}\" ${VCVARSALL_ARCH}")
      separate_arguments(VCVARSALL_CMD NATIVE_COMMAND ${VCVARSALL_CMD})

      message(STATUS " * running \'${VCVARSALL_FILE} ${VCVARSALL_ARCH}\' to set up MSVC")
      execute_process(
        COMMAND
        "cmd" "/c" ${VCVARSALL_CMD}
        "&&" "call" "echo" "VCVARSALL_ENV_START"
        "&" "set"
        OUTPUT_VARIABLE VCVARSALL_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )

      dev_prefix_substr(VCVARSALL_ENV "VCVARSALL_ENV_START" "${VCVARSALL_OUTPUT}")
      dev_env_from_str("${VCVARSALL_ENV}")
    else()
      message(SEND_ERROR " * could not find \'vcvarsall.bat\' for preparing MSVC")
    endif()
  endif()
endfunction()
