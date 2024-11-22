cmake_minimum_required(VERSION 3.30.0)
project(RayTracer VERSION 0.0.1 DESCRIPTION "ObjLoading and BVH tree testing")

include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #clangd req.

#set(CMAKE_CXX_CPPLINT "cpplint;--filter=-legal,-build/include_order,-build/include_subdir,-whitespace/indent")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(CORE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}")

FetchContent_Declare(
  tinyobjloader
  GIT_REPOSITORY  https://github.com/tinyobjloader/tinyobjloader.git
  GIT_TAG         release
  GIT_PROGRESS    TRUE
)
#set(glad_DIR "${glad_BUILD_DIR}")
#find_package(glad CONFIG REQUIRED)

FetchContent_Declare(
  tinybvh
  GIT_REPOSITORY https://github.com/jbikker/tinybvh.git
  GIT_TAG main
  GIT_PROGRESS    TRUE
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
  GIT_PROGRESS    TRUE
)

# find_package(OpenGL REQUIRED)

FetchContent_MakeAvailable(tinyobjloader tinybvh glm)
set(TINYOBJLOADER_INCLUDE_DIRS "${tinyobjloader_SOURCE_DIR}/include")
#set(GLAD_SOURCES "${glad_BINARY_DIR}/src/glad.c")
#set(TINYBVH_INCLUDE_DIRS "${tinybvh_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIRS "${glm_SOURCE_DIR}")


file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE
  ${TINYBVH_INCLUDE_DIRS}
  ${CORE_INCLUDE_DIR}   
  ${TINYOBJLOADER_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
)
#glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET}) ???
#include_directories(${INSTALL_INCLUDE_DIR})
#target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_INCLUDE_DIRS} ${CORE_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader glm::glm)


