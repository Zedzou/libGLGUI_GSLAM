cmake_minimum_required(VERSION 3.13)
project(GL_Content)
set(targetname GL_Content)

include(UseAssimp)
find_package(tinyply)
include(UseOnnxRuntime)
include(UseOpenCV)
include(UseTools)

set(sources GL_Content.cpp
            ./element/CameraDrawer.cpp
            ./element/CoordinateAxis.cpp
            ./element/SurfelDrawer.cpp
            ./element/TrajectoryDrawer.cpp
            ./dataLoader/dataLoader.cpp)

set(headers GL_Content.h
            ./element/CameraDrawer.h
            ./element/CoordinateAxis.h
            ./element/SurfelDrawer.h
            ./element/TrajectoryDrawer.h
            ./label_NYU40.h
            ./dataLoader/dataLoader.h)

include_directories(${Assimp_INCLUDE_DIR} ${TINYPLY_INCLUDE_DIR} )

add_library(${targetname} ${sources} ${headers})

include(LinkOnnxRuntime)
target_link_libraries(${targetname} PUBLIC ${Assimp_LIBRARY_DIR} 
                                    PUBLIC GraphSLAM
                                    PUBLIC GL
                                    PUBLIC GL_ContentBase)