find_package(GLM REQUIRED)
find_package(FreeType2 REQUIRED)
find_package(Eigen3)

set(PROJECT_NAME GL_ContentBase)

set(sources GL_ContentBase.cpp
            ./utils/glUtils.cpp
            ./utils/camera/projection_control.cpp)
set(headers GL_ContentBase.h
            ./utils/glUtils.hpp
            ./utils/shader/glShader.hpp
            ./utils/camera/projection_control.hpp)

add_library(${PROJECT_NAME} ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} PUBLIC GL freetype)
target_include_directories(${PROJECT_NAME} PUBLIC ${glm_INCLUDE_DIRS} PUBLIC ./utils/stb)

# FreeType
target_include_directories(${PROJECT_NAME} PUBLIC ${FreeType2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${FreeType2_LIBRARIES})

# Eigen
get_target_property(EIGEN3_INCLUDE_DIR Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DCOMPILE_WITH_EIGEN)
target_include_directories(${PROJECT_NAME} PUBLIC ${EIGEN3_INCLUDE_DIR})

# 定义GUI_FOLDER_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC GUI_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")