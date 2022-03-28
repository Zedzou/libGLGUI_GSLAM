find_package(GLM REQUIRED)
find_package(FreeType2 REQUIRED)
find_package(Eigen3)

set(sources GUI_GSLAM.cpp)
set(headers GUI_GSLAM.h)

add_library(GUI_GSLAM ${sources} ${headers})
target_link_libraries(GUI_GSLAM PUBLIC GUI)
target_include_directories(GUI_GSLAM PUBLIC ${glm_INCLUDE_DIRS} PUBLIC ./stb)

if (FreeType2_FOUND)
    message("FreeType2_library: ", ${FreeType2_LIBRARIES})
    target_include_directories(GUI_GSLAM PUBLIC ${FreeType2_INCLUDE_DIRS})
    target_link_libraries(GUI_GSLAM PUBLIC ${FreeType2_LIBRARIES})
    target_link_libraries(GUI_GSLAM PUBLIC freetype)
    target_compile_definitions(GUI_GSLAM PUBLIC -DCOMPILE_WITH_FREETYPE)
endif()

if(TARGET Eigen3::Eigen)
    get_target_property(EIGEN3_INCLUDE_DIR Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
endif()

if(EIGEN3_INCLUDE_DIR)
    target_compile_definitions(GUI_GSLAM PUBLIC -DCOMPILE_WITH_EIGEN)
    target_include_directories(GUI_GSLAM PUBLIC ${EIGEN3_INCLUDE_DIR})
endif()

target_compile_definitions(GUI_GSLAM PUBLIC GUI_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")