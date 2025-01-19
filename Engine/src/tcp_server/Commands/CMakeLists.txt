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
    CreateEntityCommand.cpp
    GetComponentOptionsCommand.cpp
    SetComponentOptionsCommand.cpp
    CreateComponentCommand.cpp
    GetLogPath.cpp
    ExportJsonCommand.cpp
    SetBouncesCommand.cpp
    GetBouncesCommand.cpp
    SetFovCommand.cpp
    GetFovCommand.cpp
    UndoCommand.cpp
    GetFrameRate.cpp
    SetFrameRate.cpp
    RemoveEntity.cpp
    RemoveComponent.cpp
)
list(TRANSFORM TCP_COMMANDS_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(TCP_COMMANDS_SOURCES ${TCP_COMMANDS_SOURCES} PARENT_SCOPE)