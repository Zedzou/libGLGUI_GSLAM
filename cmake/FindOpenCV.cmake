set(NAME OpenCV)
set(URL "git@github.com:opencv/opencv.git")
set(${NAME}_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/${NAME})

if(NOT OpenCV_FOUND)

    find_package(Git)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/external)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external)
    endif()

    # git clone 
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone ${URL} ${NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external
    )
    
    # mkdir build
    if(NOT EXISTS ${NAME}_INSTALL_DIR/build)
        file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/build)
    endif()

    # mkdir bin
    if(NOT EXISTS ${${NAME}_INSTALL_DIR}/bin)
        file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/bin)
    endif()

    # cmake -DCMAKE_INSTALL_PREFIX=${${NAME}_INSTALL_DIR}/bin
    execute_process(
        COMMAND cmake -DCMAKE_INSTALL_PREFIX=${${NAME}_INSTALL_DIR}/bin ..
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    # make
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    # make install
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    set(OpenCV_FOUND 1 CACHE STRING "set 1 to ")
endif()

set(OpenCV_INCLUDE_DIR ${${NAME}_INSTALL_DIR}/bin/include/opencv4)
set(OpenCV_LIBRARY_DIR ${${NAME}_INSTALL_DIR}/bin/lib)