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
ADD_SUBDIRECTORY(libGL_UI)

# .exe
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp 
                               libGL_Content/GL_Content.cpp 
                               libGL_Content/GL_Content.h 
                               libGL_Content/element/CameraDrawer.cpp 
                               libGL_Content/element/CameraDrawer.h
                               libGL_Content/element/CoordinateAxis.cpp
                               libGL_Content/element/CoordinateAxis.h)
target_link_libraries(${PROJECT_NAME} GL_UI)