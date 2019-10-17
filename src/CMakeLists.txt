add_library(braceexplib ${SOURCES} ${INCLUDES})

add_executable(braceexp main.cpp)
add_dependencies(braceexp braceexplib)
target_link_libraries(braceexp braceexplib)
