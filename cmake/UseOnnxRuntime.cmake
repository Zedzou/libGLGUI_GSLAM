# UseEigen3.cmake

FIND_PACKAGE(OnnxRuntime)

OPTION(WITH_ONNX "Build with libEIGEN support?" ${OnnxRuntime_FOUND})

IF(WITH_ONNX)
  include_directories(${OnnxRuntime_INCLUDE_DIRS})
ENDIF()
