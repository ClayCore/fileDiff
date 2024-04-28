function(dev_target_include_sys_dirs target)
  set(multiValueArgs INTERFACE PUBLIC PRIVATE)
  cmake_parse_arguments(
    _ARG
    ""
    ""
    "${multiValueArgs}"
    ${ARGN}
  )

  foreach(scope IN ITEMS INTERFACE PUBLIC PRIVATE)
    foreach(lib_include_dirs IN LISTS _ARG_${SCOPE})
      if(NOT MSVC)
        set(_SYSTEM SYSTEM)
      endif()

      if(${scope} STREQUAL "INTERFACE" OR ${scope} STREQUAL "PUBLIC")
        target_include_directories(
          ${target}
          ${_SYSTEM}
          ${scope}
          "$<BUILD_INTERFACE>:${lib_include_dirs}"
          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        )
      else()
        target_include_directories(
          ${target}
          ${_SYSTEM}
          ${scope}
          ${lib_include_dirs}
        )
      endif()
    endforeach()
  endforeach()
endfunction()

function(dev_target_include_sys_lib target scope lib)
  if(TARGET ${lib})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)

    if(lib_include_dirs)
      dev_target_include_sys_dirs(${target} ${scope} ${lib_include_dirs})
    else()
      message(STATUS " * \'${lib}\' does not have the INTERFACE_INCLUDE_DIRECTORIES property")
    endif()
  endif()
endfunction()

function(dev_target_link_sys_lib target scope lib)
  dev_target_include_sys_lib(${target} ${scope} ${lib})
  target_link_libraries(${target} ${scope} ${lib})
endfunction()

function(dev_target_link_sys_libs target)
  set(multiValueArgs INTERFACE PUBLIC PRIVATE)
  cmake_parse_arguments(
    _ARG
    ""
    ""
    "${multiValueArgs}"
    ${ARGN}
  )

  foreach(scope IN ITEMS INTERFACE PUBLIC PRIVATE)
    foreach(lib IN LISTS _ARG_${scope})
      dev_target_link_sys_lib(${target} ${scope} ${lib})
    endforeach()
  endforeach()
endfunction()
