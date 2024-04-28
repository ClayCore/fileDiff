add_executable(
  fileDiffCLI
  main.cpp
)
add_executable(dev::fileDiffCLI ALIAS fileDiffCLI)

target_link_libraries(fileDiffCLI PRIVATE dev_options dev_warnings)
target_link_libraries(fileDiffCLI PUBLIC libFileDiff)
target_include_directories(
  fileDiffCLI
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/bin/fileDiffCLI>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/bin/fileDiffCLI>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib>
)
target_compile_features(fileDiffCLI PUBLIC cxx_std_20)

set_target_properties(
  fileDiffCLI
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
  fileDiffCLI
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fileDiffCLI> ${PROJECT_SOURCE_DIR}/target
)
