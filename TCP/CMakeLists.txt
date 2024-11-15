cmake_minimum_required(VERSION 3.30.0)
project(tcp-test VERSION 0.0.1 DESCRIPTION "TCP Server and Client in CPP using Boost")

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio date_time)
set(BOOST_ENABLE_CMAKE ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #clangd req.
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(INSTALL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}")

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.82.0
)
FetchContent_MakeAvailable(Boost)
file(GLOB_RECURSE TCP_SERVER_SOURCES "src/tcp_server/*.cpp")
add_executable (tcp-test ${TCP_SERVER_SOURCES})
target_include_directories(tcp-test PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(tcp-test
  PRIVATE 
  Boost::asio
  # Boost::filesystem
  #Boost::thread
  # Boost::program_options
)

if (WIN32)
  target_link_libraries(tcp-test PRIVATE ws2_32 mswsock)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tcp-test PROPERTY CXX_STANDARD 20)
endif()
