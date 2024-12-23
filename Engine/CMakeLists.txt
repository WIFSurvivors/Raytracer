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
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

set(ABSOLUTE_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(ABSOLUTE_SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
set(ABSOLUTE_ASSET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/")
set(ABSOLUTE_LOG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/log/")
option(SHOW_UI "Show UI, which means WindowManager and RenderSystem are active" ON)

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
FetchContent_Declare(
  tinybvh
  # GIT_REPOSITORY https://github.com/jbikker/tinybvh.git
  GIT_REPOSITORY https://github.com/Azayel/tinybvh_raytrace_fork.git
  GIT_TAG main
  GIT_PROGRESS    TRUE
)
# find_package(OpenGL REQUIRED)


option(USE_AVX "Enable AVX and AVX2" ON)

if(USE_AVX)
# Enable AVX and AVX2
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	  add_compile_options(-mavx -mavx2)
  elseif(MSVC)
	  add_compile_options(/arch:AVX2)
  endif()
endif()



FetchContent_MakeAvailable(glad glfw glm boost tinybvh)
set(GLAD_INCLUDE_DIRS "${glad_SOURCE_DIR}/include")
set(GLFW_INCLUDE_DIRS "${glfw_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIRS "${glm_SOURCE_DIR}")
set(TINYBVH_INCLUDE_DIRS "${tinybvh_SOURCE_DIR}")

message(STATUS "Show UI: ${SHOW_UI}")
# Convert ENABLE_FEATURE to a numeric value
set(SHOW_UI_NUM $<IF:$<BOOL:${SHOW_UI}>,1,0>)
add_compile_definitions(SHOW_UI=${SHOW_UI_NUM})
add_compile_definitions(ROOT_ABSOLUTE_PATH="${ABSOLUTE_ROOT_PATH}") 
add_compile_definitions(SHADER_ABSOLUTE_PATH="${ABSOLUTE_SHADER_PATH}") 
add_compile_definitions(ASSET_ABSOLUTE_PATH="${ABSOLUTE_ASSET_PATH}") 
add_compile_definitions(LOG_ABSOLUTE_PATH="${ABSOLUTE_LOG_PATH}") 
# Add subdirectories
add_subdirectory(includes)
add_subdirectory(src)
add_executable(${PROJECT_NAME} main.cpp ${ALL_SOURCES} ${ALL_HEADERS}) 

target_include_directories(${PROJECT_NAME} PUBLIC 
#${ALL_HEADERS}
${Boost_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}
${GLAD_INCLUDE_DIRS}  
${GLFW_INCLUDE_DIRS} 
${GLM_INCLUDE_DIRS}
${tinybvh_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC  glad glfw glm::glm Boost::asio Boost::thread Boost::json Boost::uuid)
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32 mswsock)
endif()
