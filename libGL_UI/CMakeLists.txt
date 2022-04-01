find_package(GLM REQUIRED)
find_package(FreeType2 REQUIRED)
find_package(Eigen3)

set(sources GL_UI.cpp
            ./utils/glUtils.cpp
            ./utils/camera/projection_control.cpp)
set(headers GL_UI.h
            ./utils/glUtils.hpp
            ./utils/camera/glCamera.hpp
            ./utils/shader/glShader.hpp
            ./utils/mesh/glMesh.hpp)

add_library(GL_UI ${sources} ${headers})
target_link_libraries(GL_UI PUBLIC GL)
target_include_directories(GL_UI PUBLIC ${glm_INCLUDE_DIRS} PUBLIC ./utils/stb)

# FreeType
message("FreeType2_library: " ${FreeType2_LIBRARIES})
target_include_directories(GL_UI PUBLIC ${FreeType2_INCLUDE_DIRS})
target_link_libraries(GL_UI PUBLIC ${FreeType2_LIBRARIES})
target_link_libraries(GL_UI PUBLIC freetype)
target_compile_definitions(GL_UI PUBLIC -DCOMPILE_WITH_FREETYPE)

# Eigen
get_target_property(EIGEN3_INCLUDE_DIR Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
target_compile_definitions(GL_UI PUBLIC -DCOMPILE_WITH_EIGEN)
target_include_directories(GL_UI PUBLIC ${EIGEN3_INCLUDE_DIR})

# 定义GUI_FOLDER_PATH
target_compile_definitions(GL_UI PUBLIC GUI_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")