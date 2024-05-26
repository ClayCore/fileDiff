cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(NOT DEFINED CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 17)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_EXTENSIONS ON)

project(
  dev
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL "%%url%%"
  LANGUAGES CXX C
)

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/ProjectOptions.cmake)

dev_options_setup()
dev_options_global()

include(cmake/Dependencies.cmake)
dev_deps_setup()

dev_options_local()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

include(cmake/GetGitRevisionDescription.cmake)

get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 8 GIT_SHORT_SHA)

target_compile_features(dev_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_features(dev_options INTERFACE c_std_${CMAKE_C_STANDARD})

add_library(dev::dev_options ALIAS dev_options)
add_library(dev::dev_warnings ALIAS dev_warnings)

include(cmake/AddDir.cmake)
dev_add_dir("${CMAKE_SOURCE_DIR}/target")

add_subdirectory(config)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(libs)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(bin)

if(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()

if(MSVC)
  dev_get_all_installable_targets(all_targets)
  message(STATUS " * all_targets=${all_targets}")
  set_target_properties(
    ${all_targets}
    PROPERTIES
    VS_DEBUGGER_ENVIRONMENT
    "PATH=$(VC_ExecutablePath_x64);%PATH%"
  )
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT bin_tui)

if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

include(cmake/PackageProject.cmake)

dev_package_project(
  TARGETS
  bin_tui
  dev_options
  dev_warnings
)

set(CPACK_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
