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

set(RELATIVE_SHADER_PATH "./shaders/" CACHE STRING "Relative path to shader scripts")
set(RELATIVE_ASSET_PATH "./assets/" CACHE STRING "Relative path to assets (obj and mat files)")
set(RELATIVE_LOG_PATH "./logs/" CACHE STRING "Relative path to logs. This folder is created at app start")

option(SHOW_UI "Show UI, which means WindowManager and RenderSystem are active" ON)
set(FRAME_RATE_HZ 30 CACHE STRING "Determines the upper-limit for the Frame Rate")

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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-g)
endif()


FetchContent_MakeAvailable(glad glfw glm boost tinybvh)
set(GLAD_INCLUDE_DIRS "${glad_SOURCE_DIR}/include")
set(GLFW_INCLUDE_DIRS "${glfw_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIRS "${glm_SOURCE_DIR}")
set(TINYBVH_INCLUDE_DIRS "${tinybvh_SOURCE_DIR}")


# Convert ENABLE_FEATURE to a numeric value
set(SHOW_UI_NUM $<IF:$<BOOL:${SHOW_UI}>,1,0>)
add_compile_definitions(SHOW_UI=${SHOW_UI_NUM})
add_compile_definitions(FRAME_RATE_HZ=${FRAME_RATE_HZ})
add_compile_definitions(ROOT_ABSOLUTE_PATH="${ABSOLUTE_ROOT_PATH}") 

add_compile_definitions(SHADER_RELATIVE_PATH="${RELATIVE_SHADER_PATH}") 
add_compile_definitions(ASSET_RELATIVE_PATH="${RELATIVE_ASSET_PATH}") 
add_compile_definitions(LOG_RELATIVE_PATH="${RELATIVE_LOG_PATH}") 

message(STATUS "Show UI: ${SHOW_UI}")
message(STATUS "Frame Rate (Hz): ${FRAME_RATE_HZ}")

message(STATUS "Log Path (relative): ${RELATIVE_LOG_PATH}")
message(STATUS "Asset Path (relative): ${RELATIVE_ASSET_PATH}")
message(STATUS "Shader Path (relative): ${RELATIVE_SHADER_PATH}")

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

# Specify the source and destination directories
set(DEV_ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
set(DEV_SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")

# get_target_property(EXECUTABLE_PATH ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "Executable will be placed at: ${CMAKE_BINARY_DIR}")

# Define the custom command to copy folders
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEV_ASSETS_DIR}"
        "${CMAKE_BINARY_DIR}/assets"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${DEV_SHADER_DIR}"
		"${CMAKE_BINARY_DIR}/shaders"
    COMMAND ${CMAKE_COMMAND} -E echo "Copy operation completed!"
    COMMENT "Copying asset folder to the build directory"
)

# Ensure the command runs for both build systems (make/ninja/MSVC)
add_custom_target(copy_resources ALL
    DEPENDS ${PROJECT_NAME}
)
