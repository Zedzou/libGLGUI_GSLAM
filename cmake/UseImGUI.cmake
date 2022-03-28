# Find ImGUI
# Use ImGUI as an external project
#
###
SET(NAME ImGUI)
SET(URL "git@github.com:ocornut/imgui.git")
SET(${NAME}_INSTALL_DIR  ${CMAKE_BINARY_DIR}/external/${NAME})

IF(NOT EXISTS ${CMAKE_BINARY_DIR}/external/${NAME}/.github)
    IF(EXISTS ${${NAME}_INSTALL_DIR})
        file(REMOVE_RECURSE ${${NAME}_INSTALL_DIR})
    ENDIF()
    MESSAGE("cloning ${NAME}..." )
    find_package(Git)
    file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR})
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone ${URL} ${NAME}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external
    )
ELSE()
    message(${NAME} "is there and do not to git clone ")
ENDIF()


if (NOT TARGET ${NAME}-update)
    add_custom_target(${NAME}-update
            COMMENT "Updated ${NAME}"
            WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}
            COMMAND ${GIT_EXECUTABLE} pull
            #DEPENDS ${NAME}
            )
ENDIF()


if (NOT TARGET imgui)
    SET(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)
    FIND_PACKAGE(gl3w REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package( Threads REQUIRED)
    message("gl3w_SOURCE: " ${gl3w_SOURCE})
    add_library(imgui STATIC
            ${gl3w_SOURCE}
            ${ImGUI_INSTALL_DIR}/imgui.cpp
            ${ImGUI_INSTALL_DIR}/imgui_demo.cpp
            ${ImGUI_INSTALL_DIR}/imgui_draw.cpp
            ${ImGUI_INSTALL_DIR}/imgui_tables.cpp
            ${ImGUI_INSTALL_DIR}/imgui_widgets.cpp
            ${ImGUI_INSTALL_DIR}/backends/imgui_impl_glfw.cpp
            ${ImGUI_INSTALL_DIR}/backends/imgui_impl_opengl3.cpp
            )

    target_include_directories(imgui
                                PUBLIC ${gl3w_INCLUDE_DIRS}
                                PUBLIC ${ImGUI_INSTALL_DIR}
                                PUBLIC ${ImGUI_INSTALL_DIR}/backends/)

    target_link_libraries(imgui
            PUBLIC ${OPENGL_LIBRARIES}
            PUBLIC glfw
            PUBLIC dl
            PUBLIC ${CMAKE_THREAD_LIBS_INIT}
            )
            
    target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GL3W)

    if (WIN32)
        TARGET_COMPILE_DEFINITIONS(imgui PUBLIC WIN32)
    endif (WIN32)
    if (UNIX)
        TARGET_COMPILE_DEFINITIONS(imgui PUBLIC LINUX)
        TARGET_COMPILE_DEFINITIONS(imgui PUBLIC UNIX)
    endif (UNIX)

    IF (APPLE)
        TARGET_COMPILE_DEFINITIONS(imgui PUBLIC APPLE)
    endif(APPLE)
ENDIF()