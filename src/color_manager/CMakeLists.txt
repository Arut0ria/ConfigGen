set(SOURCES src/color_manager.cpp)

set(LIB_NAME color_manager)

add_library(${LIB_NAME} ${SOURCES})

# get_target_property(
#   UTILS_INCLUDE_DIRS
#   utils
#   INTERFACE_INCLUDE_DIRECTORIES
# )

target_include_directories(
  ${LIB_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  # INTERFACE ${UTILS_INCLUDE_DIRS}
)

target_link_libraries(
  ${LIB_NAME}
  PUBLIC utils
  PUBLIC config_loader
  PUBLIC yaml-cpp::yaml-cpp
)