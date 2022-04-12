set(NAME json11)
set(URL git@github.com:dropbox/json11.git)
set(${NAME}_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/${NAME})

# compile the json11
if (NOT EXISTS ${${NAME}_INSTALL_DIR})

    # git clone
    find_package(Git)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone ${URL} ${NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external
    )

    # mkdir build && mkdir bin
    if (NOT EXISTS ${${NAME}_INSTALL_DIR}/build)
        file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/build)
    endif()
    if(NOT EXISTS ${${NAME}_INSTALL_DIR})
        file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/bin)
    endif()

    # cmake -DCMAKE_INSTALL_PREFIX=
    # make 
    # make install
    execute_process(
        COMMAND cmake -DCMAKE_INSTALL_PREFIX=${${NAME}_INSTALL_DIR}/bin ..
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )
    execute_process(
        COMMAND make -j8
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )
    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${${NAME}_INSTALL_DIR}/build
    )
endif()

set(json11_include_dir ${${NAME}_INSTALL_DIR}/bin/include/x86_64-linux-gnu/)
set(json11_library_dir ${${NAME}_INSTALL_DIR}/bin/lib/x86_64-linux-gnu/libjson11.a)