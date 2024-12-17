cmake_minimum_required(VERSION 3.30.0)
project(TopLevelProject VERSION 0.0.1 DESCRIPTION "Top Level Project")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TOPLEVEL "YES")

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio date_time json uuid)
set(BOOST_ENABLE_CMAKE ON)

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

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
  GIT_PROGRESS    TRUE
)
FetchContent_MakeAvailable(glm)

# Add subdirectories
add_subdirectory(core)
add_subdirectory(TCP)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/core 
${CMAKE_CURRENT_SOURCE_DIR}/TCP
${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC core tcp)