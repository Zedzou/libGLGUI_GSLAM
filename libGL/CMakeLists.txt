INCLUDE(../cmake/UseImGUI.cmake)

SET(sources GL.cpp ../ImGuiExtension/source/tinyfiledialogs.c )
SET(headers GL.h ../ImGuiExtension/include/ImGuiExtension/tinyfiledialogs.h )

add_library(GL ${sources} ${headers})
target_link_libraries(GL PUBLIC ImGuiExtension)

# 添加测试模块
# ADD_SUBDIRECTORY(test)