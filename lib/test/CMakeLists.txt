cmake_minimum_required(VERSION 3.21)

project(libFileDiffTests LANGUAGES C)

if(PROJECT_IS_TOP_LEVEL OR TEST_INSTALLED_VERSION)
  enable_testing()

  add_library(dev::dev_options IMPORTED INTERFACE)
  add_library(dev::dev_warnings IMPORTED INTERFACE)
  add_library(dev::libFileDiff IMPORTED STATIC)

  set_target_properties(
    dev::libFileDiff PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/lib
  )

  if(NOT TARGET dev::dev_options OR NOT TARGET dev::libFileDiff)
    message(FATAL_ERROR " * required config targets not found")
    return()
  endif()
endif()

set(test_names "")
list(APPEND test_names "test_db")

list(LENGTH test_names test_names_count)
math(EXPR test_names_count "${test_names_count} - 1")

foreach(test RANGE ${test_names_count})
  list(GET test_names ${test} test_name)

  add_executable(${test_name} ${test_name}.c)

  target_include_directories(
    ${test_name}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/cheat>
  )
  target_link_libraries(
    ${test_name}
    PUBLIC
    dev::dev_options
    dev::dev_warnings
    dev::libFileDiff
  )
  add_test(${test_name} ${test_name})
endforeach()
