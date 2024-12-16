cmake_minimum_required(VERSION 3.30.0)
project(Engine VERSION 0.0.1 DESCRIPTION "Engine for TCP Server and Client in CPP using Boost")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TOPLEVEL "YES")

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio date_time json uuid)
set(BOOST_ENABLE_CMAKE ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(INSTALL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}")

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.86.0
  )
FetchContent_MakeAvailable(Boost)
  message(STATUS "Boost Include: ${Boost_INCLUDE_DIRS}")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

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

message(STATUS "Show UI: ${SHOW_UI}")
# Convert ENABLE_FEATURE to a numeric value
set(SHOW_UI_NUM $<IF:$<BOOL:${SHOW_UI}>,1,0>)
add_compile_definitions(SHOW_UI=${SHOW_UI_NUM})
add_definitions(-DSHADER_ABSOLUTE_PATH="${ABSOLUTE_SHADER_PATH}") # out-dated!
message(Status "Boost Include: ${Boost_INCLUDE_DIRS}")

# Add subdirectories
add_subdirectory(includes)
add_subdirectory(src)
add_executable(${PROJECT_NAME} main.cpp ${ALL_SOURCES} ) 

target_include_directories(${PROJECT_NAME} PUBLIC 
#${ALL_HEADERS}
${Boost_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}
${GLAD_INCLUDE_DIRS}  
${GLFW_INCLUDE_DIRS} 
${GLM_INCLUDE_DIRS}
)
message(STATUS "GLAD_INCLUDE_DIRS: ${GLAD_INCLUDE_DIRS}")
target_link_libraries(${PROJECT_NAME} PUBLIC  glad glfw glm::glm Boost::asio Boost::thread Boost::json Boost::uuid)
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32 mswsock)
endif()