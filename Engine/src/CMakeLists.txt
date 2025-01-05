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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-g)
	add_compile_options(-fimplicit-constexpr)
endif()

message(STATUS "Shader absolute path: ${ABSOLUTE_SHADER_PATH}")

add_subdirectory(tcp_server)
add_subdirectory(component)
add_subdirectory(system)
add_subdirectory(utility)
set(BASE_SOURCES
    Engine.cpp
    Entity.cpp
    Scene.cpp
    UUIDManager.cpp
    WindowManager.cpp
    AssetManager.cpp
)

list(TRANSFORM BASE_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(ALL_SOURCES ${BASE_SOURCES} 
    ${TCP_SERVER_SOURCES}
    ${SYSTEM_SOURCES}
    ${COMPONENT_SOURCES}
    ${UTILITY_SOURCES}
    PARENT_SCOPE
 )
