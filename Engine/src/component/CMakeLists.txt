set(COMPONENT_SOURCES 
    CameraComponent.cpp
    Component.cpp
    LightComponent.cpp
    RenderComponent.cpp
    SimpleComponent.cpp)

    list(TRANSFORM COMPONENT_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
    set(COMPONENT_SOURCES ${COMPONENT_SOURCES} PARENT_SCOPE)