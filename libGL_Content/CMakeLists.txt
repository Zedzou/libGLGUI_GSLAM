cmake_minimum_required(VERSION 3.13)
project(GL_Content)
set(targetname GL_Content)

find_package(Assimp REQUIRED)
find_package(OpenCV REQUIRED)

set(sources GL_Content.cpp
            ./element/CameraDrawer.cpp
            ./element/CoordinateAxis.cpp
            ./element/SurfelDrawer.cpp
            ./element/TrajectoryDrawer.cpp)

set(headers GL_Content.h
            ./element/CameraDrawer.h
            ./element/CoordinateAxis.h
            ./element/SurfelDrawer.h
            ./element/TrajectoryDrawer.h)

set(INSEG_LIB ./element/inseg_lib/libInSegLib_linux.a)

include_directories(${Assimp_INCLUDE_DIR} ${OpenCV_INCLUDE_DIR})

add_library(${targetname} ${sources} ${headers})
target_link_libraries(${targetname} PUBLIC ${Assimp_LIBRARY_DIR} ${OpenCV_LIBRARY_DIR} ${INSEG_LIB} GL GL_ContentBase)