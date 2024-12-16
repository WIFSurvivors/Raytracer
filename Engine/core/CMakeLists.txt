cmake_minimum_required(VERSION 3.30.0)
project(RayTracer VERSION 0.0.1 DESCRIPTION "RayTracer in CPP using OpenGL")

include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(COMPILE_FLAGS -g)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #clangd req.

#set(CMAKE_CXX_CPPLINT "cpplint;--filter=-legal,-build/include_order,-build/include_subdir,-whitespace/indent")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(CORE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(BOOST_INCLUDE_LIBRARIES uuid json)
set(BOOST_ENABLE_CMAKE ON)


set(ABSOLUTE_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
option(SHOW_UI "Show UI, which means WindowManager and RenderSystem are active" ON)

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

FetchContent_MakeAvailable(glad glfw glm boost)
set(GLAD_INCLUDE_DIRS "${glad_SOURCE_DIR}/include")
#set(GLAD_SOURCES "${glad_BINARY_DIR}/src/glad.c")
set(GLFW_INCLUDE_DIRS "${glfw_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIRS "${glm_SOURCE_DIR}")


file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h)
message(STATUS "Shader absolute path: ${ABSOLUTE_SHADER_PATH}")
add_definitions(-DSHADER_ABSOLUTE_PATH="${ABSOLUTE_SHADER_PATH}") # out-dated!

# THANKS CHAT GPT! <3
# Convert to numeric 0/1
message(STATUS "Show UI: ${SHOW_UI}")
# Convert ENABLE_FEATURE to a numeric value
set(SHOW_UI_NUM $<IF:$<BOOL:${SHOW_UI}>,1,0>)
add_compile_definitions(SHOW_UI=${SHOW_UI_NUM})

# add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} )
# target_include_directories(${PROJECT_NAME} PUBLIC 
# ${GLAD_INCLUDE_DIRS}
# ${CORE_INCLUDE_DIR}   
# ${GLFW_INCLUDE_DIRS}
# ${GLM_INCLUDE_DIRS}
# ${Boost_INCLUDE_DIRS}
# )
# #glad_add_library(glad STATIC API gl:core=4.6 LOCATION ${PROJECT_SOURCE_DIR}/vendor/glad-build/${TARGET}) ???
# #include_directories(${INSTALL_INCLUDE_DIR})
# #target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_INCLUDE_DIRS} ${CORE_INCLUDE_DIR} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm::glm Boost::uuid)
# 
# IF (TOPLEVEL)
#   target_link_libraries(${PROJECT_NAME} PUBLIC tcp)
# ENDIF()

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

add_library(core STATIC ${SOURCES} ${HEADERS})
target_include_directories(core PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}
${GLAD_INCLUDE_DIRS}  
${GLFW_INCLUDE_DIRS} 
${GLM_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
)
target_link_libraries(core PUBLIC glad glfw glm::glm Boost::uuid Boost::json)
IF (TOPLEVEL)
  target_link_libraries(core PUBLIC tcp)
ENDIF()
