set(SOURCES src/config_loader.cpp)

set(LIB_NAME config_loader)

add_library(
    ${LIB_NAME}
    ${SOURCES}
)

target_include_directories(
    ${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
