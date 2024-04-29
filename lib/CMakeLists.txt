include(GenerateExportHeader)

add_library(
  libFileDiff
  logging/log.c
  strbuf/str.c
  platform/win32/win32.c
  platform/unix/unix.c
)
add_library(dev::libFileDiff ALIAS libFileDiff)

target_link_libraries(libFileDiff PRIVATE dev_options dev_warnings)
target_include_directories(
  libFileDiff
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/lib>
)
target_compile_features(libFileDiff PUBLIC c_std_17)

set_target_properties(
  libFileDiff
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  C_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(
  libFileDiff
  EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/lib/libFileDiffExport.h
  BASE_NAME FD
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(libFileDiff PUBLIC -DDEV_LIB_STATIC)
else()
  target_compile_definitions(libFileDiff PUBLIC -DDEV_LIB_EXPORT)
endif()

if(WIN32)
  target_compile_definitions(libFileDiff PUBLIC -DDEV_WIN32)
else()
  target_compile_definitions(libFileDiff PUBLIC -DUNIX)
endif()

if(BUILD_TESTING)
  message(TRACE "Building tests...")
  add_subdirectory(test)
endif()

add_custom_command(
  TARGET
  libFileDiff
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:libFileDiff> ${PROJECT_SOURCE_DIR}/target
)
