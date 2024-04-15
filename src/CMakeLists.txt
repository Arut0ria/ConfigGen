set(DIRS utils config_loader color_manager)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()