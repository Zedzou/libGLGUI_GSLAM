# libGLGUI_GSLAM

## 目录结构
```
libGLGUI_GSLAM
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
|---libGUI
    |---CMakeLists.txt
    |---GUI.cpp
    |---GUI.h
|---libGUI_GSLAM
    |---CMakeLists.txt
    |---GUI_GSLAM.cpp
    |---GUI_GSLAM.h
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

