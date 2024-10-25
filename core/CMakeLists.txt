cmake_minimum_required(VERSION 3.30.0)
project(RayTracer VERSION 0.0.1 DESCRIPTION "RayTracer in CPP using OpenGL")

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #clangd req.

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(CORE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/includes")

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

FetchContent_MakeAvailable(glad glfw glm)

set(GLAD_INCLUDE_DIR "${glad_BUILD_DIR}/include")
set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIR "${glm_SOURCE_DIR}")

#find_package(glfw3 REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(glm  REQUIRED)
#find_package(glad REQUIRED)
#glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET}) ???
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
#include_directories(${INSTALL_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm glad)

