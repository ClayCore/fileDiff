function(dev_prefix_substr output prefix input)
  string(FIND "${input}" "${prefix}" PREFIX_INDEX)

  if("${PREFIX_INDEX}" STREQUAL "-1")
    message(SEND_ERROR " * could not find \'${prefix}\' in \'${input}\'")
  endif()

  string(LENGTH "${prefix}" PREFIX_LENGTH)
  math(EXPR START_INDEX "${PREFIX_INDEX} + ${PREFIX_LENGTH}")

  string(SUBSTRING "${input}" "${START_INDEX}" "-1" _OUTPUT)
  set("${output}" "${_OUTPUT}" PARENT_SCOPE)
endfunction()

function(dev_env_from_str env_string)
  string(REGEX REPLACE ";" "__sep__" ENV_STR_SEP_ADDED "${env_string}")
  string(REGEX REPLACE "\r?\n" ";" ENV_LIST ${ENV_STR_SEP_ADDED})

  foreach(_ENV_VAR ${ENV_LIST})
    string(REGEX REPLACE "=" ";" ENV_PARTS "${_ENV_VAR}")

    list(LENGTH ENV_PARTS ENV_PARTS_LENGTH)

    if("${ENV_PARTS_LENGTH}" EQUAL "2")
      list(GET ENV_PARTS 0 ENV_NAME)
      list(GET ENV_PARTS 1 ENV_VALUE)

      string(REGEX REPLACE "__sep__" ";" ENV_VALUE "${ENV_VALUE}")
      set(ENV{${ENV_NAME}} "${ENV_VALUE}")

      if("${ENV_NAME}" EQUAL "PATH")
        list(APPEND CMAKE_PROGRAM_PATH ${ENV_VALUE})
      endif()
    endif()
  endforeach()
endfunction()

function(dev_get_all_targets var)
  set(_TARGETS)
  dev_get_all_targets_recursive(_TARGETS ${CMAKE_CURRENT_SOURCE_DIR})

  set(${var} ${_TARGETS} PARENT_SCOPE)
endfunction()

function(dev_get_all_installable_targets var)
  set(_TARGETS)
  dev_get_all_targets(_TARGETS)

  foreach(_TARGET ${_TARGETS})
    get_target_property(_TARGET_TYPE ${_TARGET} TYPE)

    if(NOT ${_TARGET_TYPE} MATCHES ".*LIBRARY|EXECUTABLE")
      list(REMOVE_ITEM _TARGETS ${_TARGET})
    endif()
  endforeach()

  set(${var} ${_TARGETS} PARENT_SCOPE)
endfunction()

macro(dev_get_all_targets_recursive targets dir)
  get_property(_SUBDIRECTORIES DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)

  foreach(_SUBDIR ${_SUBDIRECTORIES})
    dev_get_all_targets_recursive(${targets} ${_SUBDIR})
  endforeach()

  get_property(_CURRENT_TARGETS DIRECTORY ${DIR} PROPERTY BUILDSYSTEM_TARGETS)
  list(APPEND ${targets} ${_CURRENT_TARGETS})
endmacro()

function(dev_is_verbose var)
  if(
    "${CMAKE_MESSAGE_LOG_LEVEL}" STREQUAL "VERBOSE"
    OR "${CMAKE_MESSAGE_LOG_LEVEL}" STREQUAL "DEBUG"
    OR "${CMAKE_MESSAGE_LOG_LEVEL}" STREQUAL "TRACE"
  )
    set(${var} ON PARENT_SCOPE)
  else()
    set(${var} OFF PARENT_SCOPE)
  endif()
endfunction()
