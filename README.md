# libGLGUI_GSLAM
    20220401版本在20220328版本的代码基础上添加相机渲染和坐标轴渲染。

## 目录结构
```
libGraphSLAM
|---GraphSLAM
|---libGL             // OpenGL渲染窗口
|---libGL_ContentBase // OpenGL进行内容渲染的基础函数,包含鼠标键盘交互,着色器解析,相机渲染函数,网格渲染函数,模型渲染函数
|---libGL_Content
```

## Build
```
mkdir build && cd build && cmake .. && make
```

