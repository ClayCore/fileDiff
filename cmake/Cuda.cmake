macro(dev_target_link_cuda target)
  set(oneValueArgs CUDA_WARNINGS)
  cmake_parse_arguments(
    _cudaArgs
    ""
    "${oneValueArgs}"
    ""
    ${ARGN}
  )

  enable_language(CUDA)

  if("${CMAKE_CUDA_STANDARD}" STREQUAL "")
    set(CMAKE_CUDA_STANDARD "${CMAKE_CXX_STANDARD}")
  endif()

  set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

  if(APPLE)
    set_property(TARGET ${target} PROPERTY BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
  endif()

  if(WIN32 AND "$ENV{VSCMD_VER}" STREQUAL "")
    message(
      WARNING
      " * compiling cuda on windows outside vs cmd or without `vcvarsall.bat x64` probably fails"
    )
  endif()

  unset(oneValueArgs)
endmacro()
