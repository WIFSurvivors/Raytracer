cmake_minimum_required(VERSION 3.30.0)
project(Engine VERSION 1.0.1 DESCRIPTION "Engine for TCP Server and Client in CPP using Boost")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TOPLEVEL "YES")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libgcc -static-libstdc++")


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
set(RELATIVE_JSON_PATH "./json/" CACHE STRING "Relative path to json files. Currently used for testing purposes.")

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

FetchContent_Declare(trestoig URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_MakeAvailable(trestoig)

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
add_compile_definitions(JSON_RELATIVE_PATH="${RELATIVE_JSON_PATH}") 

message(STATUS "Show UI: ${SHOW_UI}")
message(STATUS "Frame Rate (Hz): ${FRAME_RATE_HZ}")

message(STATUS "Log Path (relative): ${RELATIVE_LOG_PATH}")
message(STATUS "Asset Path (relative): ${RELATIVE_ASSET_PATH}")
message(STATUS "Shader Path (relative): ${RELATIVE_SHADER_PATH}")
message(STATUS "JSON Path (relative): ${RELATIVE_JSON_PATH}")

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
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

# Specify the source and destination directories
set(DEV_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(DEV_SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(DEV_JSON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/json")

# get_target_property(EXECUTABLE_PATH ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "Executable will be placed at: ${CMAKE_BINARY_DIR}")

add_custom_target(build_csharp_gui
    COMMAND dotnet publish -c Release -r win-x64 --self-contained true RaytracerGUI.csproj
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../GUI/RaytracerGUI"
    COMMENT "Building/publishing .NET GUI"
)

###
# Install-Regeln
###

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION Engine 	
    LIBRARY DESTINATION lib           
    ARCHIVE DESTINATION lib           
)


install(DIRECTORY ${DEV_ASSETS_DIR}
    DESTINATION .
    
)
install(DIRECTORY ${DEV_SHADER_DIR}
    DESTINATION .
  
)
install(DIRECTORY ${DEV_JSON_DIR}
    DESTINATION .
  
)

install(DIRECTORY	DESTINATION log
)

install(FILES 
    "C:/msys64/mingw64/bin/libwinpthread-1.dll"
    "C:/msys64/mingw64/bin/libgcc_s_seh-1.dll"
    "C:/msys64/mingw64/bin/libstdc++-6.dll"
    DESTINATION Engine
)




install(DIRECTORY
   ../GUI/RaytracerGUI/bin/Release/net8.0-windows/win-x64/publish/
    DESTINATION RaytracerGUI
)


###
# CPACK-Konfiguration
###
if (WIN32)
add_dependencies(${PROJECT_NAME} build_csharp_gui)
endif()
include(InstallRequiredSystemLibraries)
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
  SetOutPath \\\"$INSTDIR\\\\Engine\\\"
  File \\\"${CMAKE_INSTALL_PREFIX}\\\\Engine\\\\libwinpthread-1.dll\\\"
  File \\\"${CMAKE_INSTALL_PREFIX}\\\\Engine\\\\libgcc_s_seh-1.dll\\\"
  File \\\"${CMAKE_INSTALL_PREFIX}\\\\Engine\\\\libstdc++-6.dll\\\"
")

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "WIFSurvivors")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installer for ${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "...@...")

set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_MODIFY_PATH OFF)      
set(CPACK_NSIS_INSTALL_ROOT "$PROFILE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Raytracer")
# used this source: https://stackoverflow.com/questions/14831739/how-do-i-add-a-start-menu-entry-for-a-text-file-in-cpack-ans-nsis
SET( CPACK_NSIS_CREATE_ICONS_EXTRA 
"
CreateShortcut \\\"$INSTDIR\\\\Raytracer.lnk\\\" \\\"$INSTDIR\\\\RaytracerGUI\\\\RaytracerGUI.exe\\\"
 CreateShortcut \\\"$DESKTOP\\\\Raytracer.lnk\\\" \\\"$INSTDIR\\\\Raytracer.lnk\\\"
" )
set(CPACK_NSIS_EXECUTIONLEVEL "user")
# Windows-spezifische Einstellungen (z. B. NSIS):
set(CPACK_NSIS_DISPLAY_NAME "Raytracer")
set(CPACK_NSIS_HELP_LINK "https://github.com/WIFSurvivors/Raytracer")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/WIFSurvivors/Raytracer")
set(CPACK_NSIS_CONTACT "...@...")

# Einbinden
include(CPack)
