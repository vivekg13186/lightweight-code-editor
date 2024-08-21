cmake_minimum_required( VERSION 3.5 )

project( lightweight-code-editor )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_subdirectory( glfw )

option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" OFF )

if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()


include_directories("${PROJECT_SOURCE_DIR}/glad")
set( GLAD_GL "${PROJECT_SOURCE_DIR}/glad/include/glad/glad.h" )
set( KHR_GL "${PROJECT_SOURCE_DIR}//glad/include/KHR/khrplatform.h" )
set( STB_LIB "${PROJECT_SOURCE_DIR}/stb_image.h" )

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set( GLFW-CMAKE-STARTER-SRC main.cpp)
set(GLD_SRC ${PROJECT_SOURCE_DIR}/glad/src/glad.c)

add_executable( lce WIN32 ${GLFW-CMAKE-STARTER-SRC}  ${GLD_SRC} ${GLAD_GL} ${KHR_GL} ${STB_LIB}  )
target_link_libraries( lce ${OPENGL_LIBRARIES} glfw )
if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT lce )
    endif()
endif()
