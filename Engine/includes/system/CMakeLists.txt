set(SYSTEM_HEADERS
    CameraSystem.hpp
    EntityStorage.hpp
    IStorage.hpp
    RenderSystem.hpp
    SimpleSystem.hpp
    System.hpp
)
list(TRANSFORM SYSTEM_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(SYSTEM_HEADERS ${SYSTEM_HEADERS} PARENT_SCOPE)