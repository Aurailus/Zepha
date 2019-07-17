cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 14)
set (PROJECT_NAME "Zeus")

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")

project (${PROJECT_NAME})

#Include all of the library headers
include_directories (
    lib/glew/include/GL
    lib/glfw-3.3/include
    lib/glm
    lib/stb_image
    lib/cute
    lib/sol
    lib/enet/include
    lib/gzip-hpp-master/include
    lib/catch
    lib/simplex
    lib/libnoise
    /usr/include/lua5.3
)

#Include all of the dynamic libraries
set (ZEUS_LIBRARIES
    ${OPENGL_gl_LIBRARY}    #opengl
    glfw                    #glfw
    libGLEW.so              #glew
    pthread                 #std::thread
    lua5.3                  #sol2
    dl z                    #gzip
    enet                    #enet
    noise                   #libnoise
)

#Include 'Packages', I don't understand how this works
find_package(OpenGL REQUIRED)
link_directories(lib/glew/lib)

#Configure Main Executable
add_subdirectory(src)

set(MAIN_EXEC_NAME "ZeusApp")

add_executable(${MAIN_EXEC_NAME} src/Main.cpp)
target_link_libraries(${MAIN_EXEC_NAME} zeusCore)
target_link_libraries(${MAIN_EXEC_NAME} ${ZEUS_LIBRARIES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw-3.3)
target_link_libraries(ZeusApp glfw)

#Configure Test Executable
add_subdirectory(test)

set(TEST_EXEC_NAME "ZeusTest")

add_executable(${TEST_EXEC_NAME} test/Main.cpp)
target_link_libraries(${TEST_EXEC_NAME} zeusCore)
target_link_libraries(${TEST_EXEC_NAME} zeusTest)
target_link_libraries(${TEST_EXEC_NAME} ${ZEUS_LIBRARIES})
