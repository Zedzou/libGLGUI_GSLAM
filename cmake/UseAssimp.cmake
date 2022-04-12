find_package(Assimp REQUIRED)

include_directories(${Assimp_INCLUDE_DIRS})
link_libraries(${Assimp_LIBRARY_DIR})