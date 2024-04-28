add_executable(
  fileDiffGUI
  WIN32
  main.cpp
)
add_executable(dev::fileDiffGUI ALIAS fileDiffGUI)

target_link_libraries(fileDiffGUI PRIVATE dev_options dev_warnings)
target_link_libraries(fileDiffGUI PUBLIC libFileDiff)
target_include_directories(
  fileDiffGUI
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/bin/fileDiffGUI>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/bin/fileDiffGUI>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib>
)
target_compile_features(fileDiffGUI PUBLIC cxx_std_20)

set_target_properties(
  fileDiffGUI
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
  fileDiffGUI
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fileDiffGUI> ${PROJECT_SOURCE_DIR}/target
)
