find_package(OpenCV REQUIRED) # QUIET 表示查找失败不会在屏幕上显示， QUIRED表示查找失败则在屏幕上显示
include_directories(${OpenCV_INCLUDE_DIR})
link_libraries(${OpenCV_LIBRARY_DIR})
