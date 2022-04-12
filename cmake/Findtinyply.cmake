# git clone the tintyply and compile

set(NAME tinyply)
set(URL git@github.com:ddiakopoulos/tinyply.git)
set(${NAME}_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/${NAME})

if(NOT EXISTS ${${NAME}_INSTALL_DIR})

    # git clone
    find_package(Git)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/external)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external)
    endif()
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone ${URL} ${NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external
    )

    # cmake && make && make install
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/external/${NAME}/build)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external/${NAME}/build)
    endif()
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/external/${NAME}/bin)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external/${NAME}/bin)
    endif()
    execute_process(
        COMMAND cmake -DCMAKE_INSTALL_PREFIX=${${NAME}_INSTALL_DIR}/bin ..
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )
    execute_process(
        COMMAND make
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )

    set(TINYPLY_FOUND 1 CACHE STRING "set 1 to express tinyply has compiled")
endif()

set(TINYPLY_INCLUDE_DIR ${${NAME}_INSTALL_DIR}/bin/include)
set(TINYPLY_LIBRARY_DIR ${${NAME}_INSTALL_DIR}/bin/lib/libtinyply.a)
