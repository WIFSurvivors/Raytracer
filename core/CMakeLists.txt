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
#set(glad_DIR "${glad_BUILD_DIR}")
#find_package(glad CONFIG REQUIRED)

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

# find_package(OpenGL REQUIRED)

FetchContent_MakeAvailable(glad glfw glm)

set(GLAD_INCLUDE_DIRS "${glad_BINARY_DIR}/include")
set(GLAD_SOURCES "${glad_BINARY_DIR}/src/glad.c")
set(GLFW_INCLUDE_DIRS "${glfw_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIRS "${glm_SOURCE_DIR}")

#glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET}) ???
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_INCLUDE_DIRS} ${CORE_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm)

