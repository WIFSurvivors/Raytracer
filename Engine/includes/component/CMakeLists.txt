set(COMPONENT_HEADERS
    CameraComponent.hpp
    Component.hpp
    RenderComponent.hpp
    SimpleComponent.hpp
    )

    list(TRANSFORM COMPONENT_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
    set(COMPONENT_HEADERS ${COMPONENT_HEADERS} PARENT_SCOPE)