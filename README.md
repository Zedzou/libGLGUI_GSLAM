# libGLGUI_GSLAM
    通过Dear ImGui 和 OpenGL编写的带UI的渲染器

## 目录结构
```
libGL_UI
|---README.md
|---CMakeLists.txt
|---main.cpp
|---cmake
    |---FindFreeType2.cmake
    |---Findgl3w.cmake
    |---FindGLFW.cmake
    |---FindGLM.cmake
    |---UseImGUI.cmake
|---ImGuiExtension // git@github.com:Zedzou/libGLGUI.git
|---libGL // 创建OpenGL窗口--->关联OpenGL窗口与ImgUI
    |---CMakeLists.txt
    |---GL.cpp
    |---GL.h
|---libGL_UI // 定义ImgUI并进行显示
    |---CMakeLists.txt
    |---GL_UI.cpp
    |---GL_UI.h
    |---fonts // 字体
    |---Shaders // 着色器
    |---utils
        |---camera
        |---mesh
        |---PointsRenderer
        |---shader
        |---stb
        |---glUtils.hpp
        |---glUtils.cpp
```

## Build
```
mkdir build && cd build && cmake .. && make
```

