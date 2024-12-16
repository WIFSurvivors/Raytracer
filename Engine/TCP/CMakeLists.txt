cmake_minimum_required(VERSION 3.30.0)
project(tcp-test VERSION 0.0.1 DESCRIPTION "TCP Server and Client in CPP using Boost")

set(BOOST_INCLUDE_LIBRARIES thread filesystem system program_options asio date_time json uuid)
set(BOOST_ENABLE_CMAKE ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #clangd req.

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
set(INSTALL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}")


add_subdirectory(includes)
add_subdirectory(src/tcp_server)


#FetchContent_MakeAvailable(Boost)
#file(GLOB_RECURSE TCP_SERVER_SOURCES "src/tcp_server/*.cpp")


#add_library(tcp STATIC ${TCP_SERVER_SOURCES})
#message(STATUS "Boost Include: ${Boost_INCLUDE_DIRS}")
#message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
#target_include_directories(tcp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}${Boost_INCLUDE_DIRS})
#target_link_libraries(tcp
#  PUBLIC 
#  Boost::asio
  # Boost::filesystem
  #Boost::thread
  # Boost::program_options
#  Boost::json
#  Boost::uuid
#)
#if (WIN32)
#  target_link_libraries(tcp PUBLIC ws2_32 mswsock)
#endif()
#IF (TOPLEVEL) 
#  target_link_libraries(tcp PUBLIC core)
#ENDIF()

# --- Adding Google Test ---


#enable_testing()

#add_executable(test_tcp
#  tests/test_tcp.cpp
#)

#target_link_libraries(test_tcp
#  PUBLIC
#  gtest
#  gtest_main
#  Boost::asio
#  Boost::thread
#  Boost::json
#  Boost::uuid
#)
#IF(TOPLEVEL)
#  target_link_libraries(test_tcp PRIVATE core)
#ENDIF()
#add_test(NAME TCPTest COMMAND test_tcp)