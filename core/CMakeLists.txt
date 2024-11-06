cmake_minimum_required(VERSION 3.30.0)
project(RayTracer VERSION 0.0.1 DESCRIPTION "RayTracer in CPP using OpenGL")

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #clangd req.

set(CMAKE_CXX_CPPLINT "cpplint;--filter=-legal,-build/include_order,-build/include_subdir,-whitespace/indent")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(INSTALL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/includes")

FetchContent_Declare(
  glad
  GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
  GIT_TAG         v0.1.36
  GIT_PROGRESS    TRUE
)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
  GIT_PROGRESS    TRUE
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
  GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(glm glad)
#glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET}) ???
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${INSTALL_INCLUDE_DIR})
add_executable(RayTracer ${SOURCES})
target_link_libraries(RayTracer PRIVATE glfw glm::glm glad)

