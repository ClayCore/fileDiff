include(GenerateExportHeader)

# Create library target and add all the sources
add_library(
  lib_core
  logging/log.c
  hash/hash_file.c
  db/db.c
  db/db_write.c
  db/db_read.c
  $<$<PLATFORM_ID:Windows>:
  platform/win32/file.c
  platform/win32/path.c
  >
  $<$<PLATFORM_ID:Linux>:
  platform/unix/unix.c
  >
)

# Utility alias
add_library(dev::lib_core ALIAS lib_core)

# Link OpenSSL with the library
target_link_libraries(lib_core PUBLIC OpenSSL::Crypto)

# Add global `dev_*` dependencies
target_link_libraries(lib_core PRIVATE dev_options dev_warnings)

# Add the main `lib_core` directory
target_include_directories(
  lib_core
  ${WARNING_GUARD}
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs/lib_core>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_core>
)
target_compile_features(lib_core PUBLIC c_std_17)

# Ensure anything marked hidden doesn't get exported in the shared library
set_target_properties(
  lib_core
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  C_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES
)

# Generate the export header
generate_export_header(
  lib_core
  EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/libs/lib_core/lib_core_export.h
  BASE_NAME FD
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(lib_core PUBLIC -DDEV_LIB_STATIC)
else()
  target_compile_definitions(lib_core PUBLIC -DDEV_LIB_EXPORT)
endif()

if(WIN32)
  target_compile_definitions(lib_core PUBLIC -DDEV_WIN32)

  # Disable msvcrt nonsense
  target_compile_definitions(
    lib_core
    PRIVATE
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_DEPRECATE
    -D_CRT_NONSTDC_NO_WARNINGS
  )
else()
  target_compile_definitions(lib_core PUBLIC -DDEV_UNIX)
endif()

include(CTest)

if(BUILD_TESTING AND PROJECT_IS_TOP_LEVEL)
  message(AUTHOR_WARNING "Building tests for lib_core...")
  add_subdirectory(test)
endif()

# Allows us to run the executable and test binaries after
# the build is finished, otherwise the built shared library
# would be missing
add_custom_command(
  TARGET lib_core
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lib_core> ${PROJECT_SOURCE_DIR}/target
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lib_core> $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/libs/lib_core/test/>
)
