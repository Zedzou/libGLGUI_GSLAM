cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME GraphSLAM)

set(INSEG_LIB ./inseg_lib/libInSegLib_linux.a)

file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

include(UseOnnxRuntime)
file(GLOB sources_gp "graphpredictor/*.cpp")
file(GLOB headers_gp "graphpredictor/*.h")

set(sources ${sources} ${sources_gp})
set(headers ${headers} ${headers_gp})

add_library(${PROJECT_NAME} ${sources} ${headers})

target_link_libraries(${PROJECT_NAME} PUBLIC ${INSEG_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ./inseg_lib/include)

include(LinkOnnxRuntime)
include(Linktinyply)
include(LinkJson)