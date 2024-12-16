add_subdirectory(utility)
add_subdirectory(tcp_server)
add_subdirectory(system)
add_subdirectory(component)
set(BASE_HEADERS
    Engine.hpp
    Entity.hpp
    Scene.hpp
    ShaderCompiler.hpp
    UUIDManager.hpp
    WindowManager.hpp
)

list(TRANSFORM BASE_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(ALL_HEADERS ${BASE_HEADERS}     
    ${UTILITY_HEADERS}
    ${TCP_SERVER_HEADERS}
    ${SYSTEM_HEADERS}
    ${COMPONENT_HEADERS}
    )
    message(STATUS "ALL_HEADERS: ${ALL_HEADERS}")