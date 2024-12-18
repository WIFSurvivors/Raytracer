set(TCP_COMMANDS_SOURCES 
    CloseRenderCommand.cpp
    GetRootCommand.cpp
    GetChildEntitiesCommand.cpp
    GetComponentsCommand.cpp
    GetEntityOptionsCommand.cpp
    MoveCommand.cpp
    RotateCommand.cpp
    ScaleCommand.cpp
    ImportJsonCommand.cpp
)
list(TRANSFORM TCP_COMMANDS_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(TCP_COMMANDS_SOURCES ${TCP_COMMANDS_SOURCES} PARENT_SCOPE)