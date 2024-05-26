add_executable(
  bin_tui
  main.cpp
)
add_executable(dev::bin_tui ALIAS bin_tui)

target_link_libraries(bin_tui PRIVATE dev_options dev_warnings)
target_link_libraries(bin_tui PUBLIC lib_core)
target_include_directories(
  bin_tui
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/bin/bin_tui>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/lib_core>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/bin/bin_tui>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_core>
)
target_compile_features(bin_tui PUBLIC cxx_std_20)

set_target_properties(
  bin_tui
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES
)

if(BUILD_TESTING)
  message(TRACE "Building tests...")
  add_subdirectory(test)
endif()

add_custom_command(
  TARGET
  bin_tui
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bin_tui> ${PROJECT_SOURCE_DIR}/target
)
