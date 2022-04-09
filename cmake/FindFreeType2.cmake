SET(NAME FreeType2)
SET(URL "git@github.com:Zedzou/FreeType2.git")
SET(${NAME}_INSTALL_DIR  ${CMAKE_BINARY_DIR}/external/${NAME})

# 编译FreeType2
IF(NOT ${NAME}_DOWNLOADED)

        find_package(Git)
        IF(NOT EXISTS ${${NAME}_INSTALL_DIR})
                file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR})
        ENDIF()

        # git clone 
        execute_process(
                COMMAND ${GIT_EXECUTABLE} clone ${URL} ${NAME}
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external
        )

        # mkdir build
        if (NOT EXISTS ${${NAME}_INSTALL_DIR}/build)
                file(MAKE_DIRECTORY ${${NAME}_INSTALL_DIR}/build)
        endif()

        # mkdir bin
        if (NOT EXISTS ${${NAME}_INSTALL_DIR}/bin)
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

        set(${NAME}_DOWNLOADED 1 CACHE STRING "Set 1")
  
ENDIF()

# 设置FreeType2 路径
FIND_PACKAGE(freetype REQUIRED PATHS ${${NAME}_INSTALL_DIR}/bin/lib/cmake NO_DEFAULT_PATH)