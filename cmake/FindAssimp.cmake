cmake_minimum_required(VERSION 3.13)

set(NAME Assimp)
set(URL "git@github.com:Zedzou/Assimp.git")
set(${NAME}_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/${NAME})

if(NOT EXISTS ${${NAME}_INSTALL_DIR})

    # git clone 
    find_package(Git)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone ${URL} ${NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external
    )

    # mkdir build
    if(NOT EXISTS ${${NAME}_INSTALL_DIR}/build)
        file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/build)
    endif()

    # mkdir bin
    if(NOT EXISTS ${${NAME}_INSTALL_DIR}/bin)
        file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/bin)
    endif()

    # cmake ..
    message(CMAKE_Assimp "cmake the Assimp at ${NAME}_INSTALL_DIR/build")
    execute_process(
        COMMAND cmake -DCMAKE_INSTALL_PREFIX=${${NAME}_INSTALL_DIR}/bin ..
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    # make
    message(CMAKE_Assimp "make the Assimp at ${NAME}_INSTALL_DIR/build")
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    message(CMAKE_Assimp "make install the Assimp at ${NAME}_INSTALL_DIR/build")
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    set(Assimp_FOUND 1 CACHE STRING "set to 1 if Assimp is found, otherwise 0")
endif()

set(Assimp_INCLUDE_DIRS ${NAME}_INSTALL_DIR/bin/assimp/incude CACHE STRING " Assimp include directories")
set(Assimp_LIBRARY_DIR ${${NAME}_INSTALL_DIR}/bin/lib/libassimp.so
                       ${${NAME}_INSTALL_DIR}/bin/lib/libassimp.so.5
                       ${${NAME}_INSTALL_DIR}/bin/lib/libassimp.so.5.1.4)

