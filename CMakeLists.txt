cmake_minimum_required(VERSION 3.5.0)
project(ConfigGen VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

add_executable(ConfigGen main.cpp)

target_compile_definitions(
  ConfigGen
  PRIVATE -DSTB_IMAGE_IMPLEMENTATION
)

add_subdirectory(src)

target_include_directories(
  ConfigGen
  PRIVATE include
)
  
target_link_libraries(
  ConfigGen
  PUBLIC yaml-cpp::yaml-cpp
  PRIVATE utils
  PRIVATE config_loader
  PRIVATE color_manager
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
