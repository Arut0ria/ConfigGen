set(LIB_NAME utils)

set(SOURCES src/hsl.cpp src/rgb.cpp src/hsl_operators.cpp src/rgb_operators.cpp)

add_library(${LIB_NAME} ${SOURCES})

target_include_directories(
    ${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(
    ${LIB_NAME}_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE
)