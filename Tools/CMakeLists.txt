set(TARGET_NAME Tools)
INCLUDE(UseCUDA)
INCLUDE(UseOpenMP)

file(GLOB_RECURSE srcs *.cpp)
file(GLOB_RECURSE header *.hpp *.h)
file(GLOB_RECURSE cuda *.cu)

SET(sources ${srcs} ${cuda})
SET(headers ${header})
SOURCE_GROUP("" FILES ${sources} ${headers})

ADD_LIBRARY(${targetname} ${sources} ${headers})
target_include_directories(${targetname} PUBLIC ./include)
set_target_properties(${targetname} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${targetname} PROPERTIES CXX_STANDARD 11)