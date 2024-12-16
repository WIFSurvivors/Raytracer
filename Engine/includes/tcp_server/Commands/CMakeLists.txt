set(TCP_COMMANDS_HEADERS
    CloseRenderCommand.hpp
    GetChildEntitiesCommand.hpp
    GetComponentsCommand.hpp
    GetEntityOptionsCommand.hpp
    GetRootCommand.hpp
    MoveCommand.hpp
    RotateCommand.hpp
)

list(TRANSFORM TCP_COMMANDS_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(TCP_COMMANDS_HEADERS ${TCP_COMMANDS_HEADERS} PARENT_SCOPE)