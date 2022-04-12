cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME GraphSLAM)

# 第三方库
find_package(OnnxRuntime REQUIRED)
find_package(tinyply REQUIRED)
find_package(Json REQUIRED)
find_package(Eigen3)
include(UseOnnxRuntime)
include(UseOpenCV)
include(UseTools)
include(UseInsegLib)

# sources and headers
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
file(GLOB sources_gp "graphpredictor/*.cpp")
file(GLOB headers_gp "graphpredictor/*.h")
set(cpp ${sources} ${sources_gp})
set(hpp ${headers} ${headers_gp})

# 编译设置
add_library(${PROJECT_NAME} ${cpp} ${hpp})


target_link_libraries(${PROJECT_NAME} 
                      PUBLIC ${TINYPLY_LIBRARY_DIR} 
                      PUBLIC ${json11_library_dir})

target_include_directories(${PROJECT_NAME} 
                            PUBLIC ${TINYPLY_INCLUDE_DIR} 
                            PUBLIC ${json11_include_dir})