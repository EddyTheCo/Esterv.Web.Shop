cmake_minimum_required(VERSION 3.24)

option(BUILD_CLIENT "Build Client part" OFF)
option(BUILD_SERVER "Build Server part" OFF)
option(BUILD_DOCS "Build Documentation part" OFF)
option(BUILD_TESTS "Build TESTS part" OFF)
option(QTDEPLOY "Deploy Qt libraries" OFF)

include(FetchContent)
FetchContent_Declare(
  ccommon
  GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
  GIT_TAG v0.5.0)
FetchContent_MakeAvailable(ccommon)
version_from_git(LOG OFF TIMESTAMP "%Y%m%d%H%M%S")
project(
  ProductWeb
  VERSION ${VERSION}
  DESCRIPTION ""
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)

find_package(Qt6 COMPONENTS Core)
add_library(Core include/object_core.hpp src/object_core.cpp include/product.hpp src/product.cpp)
target_include_directories(
  Core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/>")
target_link_libraries(Core PUBLIC Qt6::Core)

if(BUILD_SERVER)

endif(BUILD_SERVER)

if(BUILD_CLIENT)

endif(BUILD_CLIENT)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  build_cpack()
  if(BUILD_TESTS)
    include(CTest)
    add_subdirectory(tests)
  endif(BUILD_TESTS)
  if(BUILD_DOCS)
    build_docs()
  endif(BUILD_DOCS)
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
