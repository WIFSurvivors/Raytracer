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
target_include_directories(tcp-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

target_link_libraries(tcp-test
  PRIVATE 
  Boost::asio
  # Boost::filesystem
  #Boost::thread
  # Boost::program_options
  Boost::json
  core
)

if (WIN32)
  target_link_libraries(tcp-test PRIVATE ws2_32 mswsock)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tcp-test PROPERTY CXX_STANDARD 20)
endif()

add_library(tcp STATIC ${TCP_SERVER_SOURCES})
target_include_directories(tcp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(tcp
  PRIVATE 
  Boost::asio
  # Boost::filesystem
  #Boost::thread
  # Boost::program_options
  Boost::json
  core
)
if (WIN32)
  target_link_libraries(tcp PRIVATE ws2_32 mswsock)
endif()


# --- Adding Google Test ---

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_tcp
  tests/test_tcp.cpp
)

target_link_libraries(test_tcp
  PRIVATE
  gtest
  gtest_main
  Boost::asio
  Boost::thread
)

add_test(NAME TCPTest COMMAND test_tcp)