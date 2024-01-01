cmake_minimum_required(VERSION 3.0)

set(NAME ComputerGraphicsAlgo)
set(VCPKG_PATH "C:/src/vcpkg/installed/x64-windows")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${VCPKG_PATH}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${VCPKG_PATH}/lib)

list(APPEND CMAKE_PREFIX_PATH "C:/src/vcpkg/installed/x64-windows")
file(GLOB src
     "*.h"
     "*.cpp"
     "./shader/*.*"
)   

project(${NAME})

add_executable(${NAME} ${src})

target_link_libraries(${NAME} PRIVATE opengl32)
target_link_libraries(${NAME} PRIVATE glad)
target_link_libraries(${NAME} PRIVATE glfw3)
target_link_libraries(${NAME} PRIVATE glfw3dll)
target_link_libraries(${NAME} PRIVATE assimp-vc143-mt)

