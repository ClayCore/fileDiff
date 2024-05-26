add_executable(
  bin_gui
  WIN32
  main.cpp
)
add_executable(dev::bin_gui ALIAS bin_gui)

target_link_libraries(bin_gui PRIVATE dev_options dev_warnings)
target_link_libraries(bin_gui PUBLIC lib_core)
target_include_directories(
  bin_gui
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/bin/bin_gui>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/lib_core>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/bin/bin_gui>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_core>
)
target_compile_features(bin_gui PUBLIC cxx_std_20)

set_target_properties(
  bin_gui
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
  bin_gui
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bin_gui> ${PROJECT_SOURCE_DIR}/target
)
