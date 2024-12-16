set(TCP_COMMANDS_SOURCES 
    CloseRendererCommand.cpp
    GetRootCommand.cpp
    GetChildEntitiesCommand.cpp
    GetComponentsCommand.cpp
    GetEntityOptionsCommand.cpp
    MoveCommand.cpp
    RotateCommand.cpp
)
list(TRANSFORM TCP_COMMANDS_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(TCP_COMMANDS_SOURCES ${TCP_COMMANDS_SOURCES} PARENT_SCOPE)