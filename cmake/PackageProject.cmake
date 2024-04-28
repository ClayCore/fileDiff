function(dev_package_project)
  cmake_policy(SET CMP0103 NEW)

  set(_options ARCH_INDEPENDENT)
  include(GNUInstallDirs)

  # region "Properties"
  set(
    oneValueArgs
    NAME
    COMPONENT
    VERSION
    COMPATIBILITY
    CONFIG_EXPORT_DESTINATION
    CONFIG_INSTALL_DESTINATION
  )
  set(
    multiValueArgs
    TARGETS
    PUBLIC_INCLUDES
    PUBLIC_DEPENDENCIES_CONFIGURED
    PUBLIC_DEPENDENCIES
    PRIVATE_DEPENDENCIES_CONFIGURED
    PRIVATE_DEPENDENCIES
  )

  cmake_parse_arguments(
    packageProject
    "${_options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    "${ARGN}"
  )

  # endregion "Properties"

  # region "packageProject properties"
  if(NOT packageProject_TARGETS)
    dev_get_all_installable_targets(packageProject_TARGETS)
    message(INFO " * packageProject: considering ${packageProject_TARGETS} as exported targets")
  endif()

  if("${packageProject_NAME}" STREQUAL "")
    set(packageProject_NAME ${PROJECT_NAME})
  endif()

  set(packageProject_NAMESPACE "${packageProject_NAME}::")

  set(packageProject_EXPORT ${packageProject_NAME})

  set(packageProject_VARS_PREFIX ${packageProject_NAME})

  if("${packageProject_VERSION}" STREQUAL "")
    set(packageProject_VERSION ${PROJECT_VERSION})
  endif()

  if("${packageProject_COMPATIBILITY}" STREQUAL "")
    set(packageProject_COMPATIBILITY "SameMajorVersion")
  endif()

  if("${packageProject_CONFIG_EXPORT_DESTINATION}" STREQUAL "")
    set(packageProject_CONFIG_EXPORT_DESTINATION "${CMAKE_BINARY_DIR}")
  endif()

  set(packageProject_EXPORT_DESTINATION "${packageProject_CONFIG_EXPORT_DESTINATION}")

  if("${packageProject_CONFIG_INSTALL_DESTINATION}" STREQUAL "")
    set(packageProject_CONFIG_INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/${packageProject_NAME}")
  endif()

  set(packageProject_INSTALL_DESTINATION "${packageProject_CONFIG_INSTALL_DESTINATION}")

  if(NOT "${packageProject_PUBLIC_INCLUDES}" STREQUAL "")
    foreach(_INC ${packageProject_PUBLIC_INCLUDES})
      if(NOT IS_ABSOLUTE ${_INC})
        set(_INC "${CMAKE_CURRENT_SOURCE_DIR}/${_INC}")
      endif()

      if(IS_DIRECTORY ${_INC})
        install(DIRECTORY ${_INC} DESTINATION "./")
      else()
        install(FILES ${_INC} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
      endif()
    endforeach()
  endif()

  if(NOT "${packageProject_PUBLIC_DEPENDENCIES_CONFIGURED}" STREQUAL "")
    set(_PUBLIC_DEPENDENCIES_CONFIG)

    foreach(_DEP ${packageProject_PUBLIC_DEPENDENCIES_CONFIGURED})
      list(APPEND _PUBLIC_DEPENDENCIES_CONFIG "${_DEP} CONFIG")
    endforeach()
  endif()

  list(APPEND packageProject_PUBLIC_DEPENDENCIES ${_PUBLIC_DEPENDENCIES_CONFIG})
  set(packageProject_DEPENDENCIES ${packageProject_PUBLIC_DEPENDENCIES})

  if(NOT "${packageProject_PRIVATE_DEPENDENCIES_CONFIGURED}" STREQUAL "")
    set(_PRIVATE_DEPENDENCIES_CONFIG)

    foreach(_DEP ${packageProject_PRIVATE_DEPENDENCIES_CONFIGURED})
      list(APPEND _PRIVATE_DEPENDENCIES_CONFIG "${_DEP} CONFIG")
    endforeach()
  endif()

  list(APPEND packageProject_PRIVATE_DEPENDENCIES ${_PRIVATE_DEPENDENCIES_CONFIG})

  # endregion "packageProject properties"

  # region "Installation"
  # message(AUTHOR_WARNING "name=${packageProject_NAME}")
  # message(AUTHOR_WARNING "targets=${packageProject_TARGETS}")
  # message(AUTHOR_WARNING "export=${packageProject_EXPORT}")
  install(
    TARGETS ${packageProject_TARGETS}
    EXPORT ${packageProject_EXPORT}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shlib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bin
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${packageProject_NAME}" COMPONENT dev
  )

  set(_TARGETS_STR "")

  foreach(_TARGET ${packageProject_TARGETS})
    set(_TARGETS_STR "${_TARGETS_STR} ${packageProject_NAMESPACE}${_TARGET}")
  endforeach()

  set(
    USAGE_FILE_CONTENT
    " * The package ${packageProject_NAME} provides CMake targets:

      find_package(${packageProject_NAME} CONFIG REQUIRED)
      target_link_libraries(main PRIVATE ${_TARGETS_STR})"
  )

  install(CODE "MESSAGE(STATUS \"${USAGE_FILE_CONTENT}\")")
  file(WRITE "${packageProject_EXPORT_DESTINATION}/usage" "${USAGE_FILE_CONTENT}")
  install(
    FILES
    "${packageProject_EXPORT_DESTINATION}/usage"
    DESTINATION
    "${packageProject_CONFIG_INSTALL_DESTINATION}"
  )

  unset(packageProject_TARGETS)

  # endregion "Installation"

  # region "ForwardArguments and YCM"
  FetchContent_Declare(
    _fargs
    URL
    https://github.com/polysquare/cmake-forward-arguments/archive/8c50d1f956172edb34e95efa52a2d5cb1f686ed2.zip
  )
  FetchContent_GetProperties(_fargs)

  if(NOT _fargs_POPULATED)
    FetchContent_Populate(_fargs)
  endif()

  include("${_fargs_SOURCE_DIR}/ForwardArguments.cmake")

  set(_fargs_LIST)
  cmake_forward_arguments(
    packageProject
    _fargs_LIST
    OPTION_ARGS
    "${_options};"
    SINGLEVAR_ARGS
    "${oneValueArgs};EXPORT_DESTINATION;INSTALL_DESTINATION;NAMESPACE;VARS_PREFIX;EXPORT"
    MULTIVAR_ARGS
    "${multiValueArgs};DEPENDENCIES;PRIVATE_DEPENDENCIES"
  )

  FetchContent_Declare(
    _ycm
    URL
    https://github.com/robotology/ycm/archive/refs/tags/v0.13.0.zip
  )
  FetchContent_GetProperties(_ycm)

  if(NOT _ycm_POPULATED)
    FetchContent_Populate(_ycm)
  endif()

  include("${_ycm_SOURCE_DIR}/modules/InstallBasicPackageFiles.cmake")

  install_basic_package_files(${packageProject_NAME} "${_fargs_LIST}")

  include("${_ycm_SOURCE_DIR}/modules/AddUninstallTarget.cmake")

  # endregion "ForwardArguments and YCM"
endfunction()
