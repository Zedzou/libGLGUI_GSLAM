#
# Try to find onnxruntime library and include path.
# Once done this will define
# OnnxRuntime_FOUND
# ONNXROOT_DIR
# OnnxRuntime_INCLUDE_DIRS
# OnnxRuntime_LIBRARY
#
# And will create an interface library
# OnnxRuntime

# 从系统中找到OnnxRuntime的路径
FIND_PATH(ONNXROOT_DIR NAMES ./include/onnxruntime/core/session/onnxruntime_cxx_api.h
        PATHS
        /usr/local/
        NO_DEFAULT_PATH
)

# 设置include路径和library路径
IF(ONNXROOT_DIR)
    SET(OnnxRuntime_INCLUDE_DIRS  ${ONNXROOT_DIR}/include ${ONNXROOT_DIR}/include/onnxruntime/core/session)
    FIND_LIBRARY(OnnxRuntime_LIBRARY
                 NAMES
                 libonnxruntime.dylib
                 libonnxruntime.so
                 PATHS
                 ${ONNXROOT_DIR}/lib)
ENDIF(ONNXROOT_DIR)

# 设置OnnxRuntime_FOUND
IF (ONNXROOT_DIR AND OnnxRuntime_LIBRARY)
    SET( OnnxRuntime_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ELSE (ONNXROOT_DIR AND OnnxRuntime_LIBRARY)
    SET( OnnxRuntime_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ENDIF (ONNXROOT_DIR AND OnnxRuntime_LIBRARY)
MARK_AS_ADVANCED( OnnxRuntime_FOUND )

# 设置环境
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OnnxRuntime REQUIRED_VARS OnnxRuntime_LIBRARY OnnxRuntime_INCLUDE_DIRS)
if(OnnxRuntime_FOUND)
    if(NOT TARGET OnnxRuntime)
        add_library(OnnxRuntime UNKNOWN IMPORTED)
        set_target_properties(OnnxRuntime PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OnnxRuntime_INCLUDE_DIRS}")
        set_target_properties(OnnxRuntime PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES "CXX" IMPORTED_LOCATION "${OnnxRuntime_LIBRARY}")
        TARGET_COMPILE_DEFINITIONS(OnnxRuntime INTERFACE COMPILE_WITH_ONNX)
    endif()
endif()