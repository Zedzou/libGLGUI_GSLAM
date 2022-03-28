INCLUDE(../cmake/UseImGUI.cmake)

SET(sources GUI.cpp ../ImGuiExtension/source/tinyfiledialogs.c )
SET(headers GUI.h ../ImGuiExtension/include/ImGuiExtension/tinyfiledialogs.h )

add_library(GUI ${sources} ${headers})
target_link_libraries(GUI PUBLIC ImGuiExtension)

# 添加测试模块
# ADD_SUBDIRECTORY(test)