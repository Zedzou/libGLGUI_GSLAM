cmake_minimum_required(VERSION 3.13)
project(libGLGUI)
set(${PROJECT_NAME} LibGLGUI)

# CMAKE_CXX_STANDER
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# 子模块
ADD_SUBDIRECTORY(ImGuiExtension)
ADD_SUBDIRECTORY(libGUI)
add_subdirectory(libGUI_GSLAM)

# .exe
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} GUI_GSLAM)