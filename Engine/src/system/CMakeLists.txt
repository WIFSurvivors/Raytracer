set(SYSTEM_SOURCES 
    CameraSystem.cpp
    EntityStorage.cpp
    IStorage.cpp
	LightSystem.cpp
    RenderSystem.cpp
    SimpleSystem.cpp
    System.cpp
)
list(TRANSFORM SYSTEM_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(SYSTEM_SOURCES ${SYSTEM_SOURCES} PARENT_SCOPE)