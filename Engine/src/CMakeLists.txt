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




#file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
#file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h)
message(STATUS "Shader absolute path: ${ABSOLUTE_SHADER_PATH}")

# THANKS CHAT GPT! <3
# Convert to numeric 0/1

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

#add_library(core STATIC ${SOURCES} ${HEADERS})
#target_include_directories(core PUBLIC 
#${CMAKE_CURRENT_SOURCE_DIR}
#${GLAD_INCLUDE_DIRS}  
#${GLFW_INCLUDE_DIRS} 
#${GLM_INCLUDE_DIRS}
#${Boost_INCLUDE_DIRS}
#)
#target_link_libraries(core PUBLIC glad glfw glm::glm Boost::uuid Boost::json)
#IF (TOPLEVEL)
#  target_link_libraries(core PUBLIC tcp)
#ENDIF()

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
)
list(TRANSFORM BASE_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
message(STATUS "BASE_SOURCES: ${BASE_SOURCES}")
message(STATUS "TCP_SERVER_SOURCES: ${TCP_SERVER_SOURCES}")
message(STATUS "SYSTEM_SOURCES: ${SYSTEM_SOURCES}")
message(STATUS "COMPONENT_SOURCES: ${COMPONENT_SOURCES}")
message(STATUS "UTILITY_SOURCES: ${UTILITY_SOURCES}")
set(ALL_SOURCES ${BASE_SOURCES} 
    ${TCP_SERVER_SOURCES}
    ${SYSTEM_SOURCES}
    ${COMPONENT_SOURCES}
    ${UTILITY_SOURCES}
    PARENT_SCOPE
 )
