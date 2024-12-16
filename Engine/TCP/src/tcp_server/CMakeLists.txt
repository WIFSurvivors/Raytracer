add_subdirectory(ComandsImplementations)

set(TCP_SERVER_SOURCES
    CommandManager.cpp
    GetRootCommand.cpp
    TcpExecuter.cpp
    TcpServer.cpp
    TcpParser.cpp
    ParsedTcpCommand.cpp
    TcpCommandFactory.cpp
    ${TCP_COMMANDS_SOURCES}
)


list(TRANSFORM TCP_SERVER_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(TCP_SERVER_SOURCES ${TCP_SERVER_SOURCES} PARENT_SCOPE)