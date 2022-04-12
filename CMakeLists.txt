cmake_minimum_required(VERSION 3.13)
project(GLUI)
set(${PROJECT_NAME} GLUI)

# CMAKE_CXX_STANDER
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# 子模块
ADD_SUBDIRECTORY(ImGuiExtension)
ADD_SUBDIRECTORY(libGL)
ADD_SUBDIRECTORY(libGL_ContentBase)
add_subdirectory(GraphSLAM)
add_subdirectory(libGL_Content)

# 依赖
include(UseOpenCV)
include(UseTools)
include(UseOnnxRuntime)

# .exe
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp )
target_link_libraries(${PROJECT_NAME} 
                      PUBLIC GL_Content 
                      PUBLIC GraphSLAM
                      PUBLIC GL 
                      PUBLIC GL_ContentBase )