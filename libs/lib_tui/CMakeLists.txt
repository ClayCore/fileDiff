include(GenerateExportHeader)

# Create library target and add all the sources
add_library(
  lib_tui
  main.cpp
)

# Utility alias
add_library(dev::lib_tui ALIAS lib_tui)

# Link OpenSSL with the library
target_link_libraries(lib_tui PUBLIC lib_core)

# Add global `dev_*` dependencies
target_link_libraries(lib_tui PRIVATE dev_options dev_warnings)

# Add the main `lib_tui` directory
target_include_directories(
  lib_tui
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/lib_tui>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_tui>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/lib_core>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_core>
)
target_compile_features(lib_tui PUBLIC cxx_std_20)

# Ensure anything marked hidden doesn't get exported in the shared library
set_target_properties(
  lib_tui
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES
)

# Generate the export header
generate_export_header(
  lib_tui
  EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/libs/lib_tui/lib_tui_export.hpp
  BASE_NAME FD_TUI
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(lib_tui PUBLIC -DDEV_LIB_STATIC)
else()
  target_compile_definitions(lib_tui PUBLIC -DDEV_LIB_EXPORT)
endif()

if(WIN32)
  target_compile_definitions(lib_tui PUBLIC -DDEV_WIN32)

  # Disable msvcrt nonsense
  target_compile_definitions(
    lib_tui
    PRIVATE
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_DEPRECATE
    -D_CRT_NONSTDC_NO_WARNINGS
  )
else()
  target_compile_definitions(lib_tui PUBLIC -DDEV_UNIX)
endif()

include(CTest)

if(BUILD_TESTING AND PROJECT_IS_TOP_LEVEL)
  message(AUTHOR_WARNING "Building tests for lib_tui...")
  add_subdirectory(test)
endif()

# Allows us to run the executable and test binaries after
# the build is finished, otherwise the built shared library
# would be missing
add_custom_command(
  TARGET lib_tui
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lib_tui> ${PROJECT_SOURCE_DIR}/target
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lib_tui> $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_tui/test/>
)
