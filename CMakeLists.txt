cmake_minimum_required(VERSION 3.30.0)
project(TopLevelProject VERSION 0.0.1 DESCRIPTION "Top Level Project")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TOPLEVEL "YES")
# Add subdirectories
add_subdirectory(core)
add_subdirectory(TCP)

add_executable(${PROJECT_NAME} build/core/libcore.a build/TCP/libtcp.a)
target_include_directories(${PROJECT_NAME} PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/core 
${CMAKE_CURRENT_SOURCE_DIR}/TCP
)
target_link_libraries(${PROJECT_NAME} PRIVATE core tcp)