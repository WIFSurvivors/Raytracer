add_subdirectory(Commands)

set(TCP_SERVER_SOURCES
    CommandManager.cpp
    TcpExecuter.cpp
    TcpServer.cpp
    TcpParser.cpp
    ParsedTcpCommand.cpp
    TcpCommandFactory.cpp
)


list(TRANSFORM TCP_SERVER_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(TCP_SERVER_SOURCES ${TCP_SERVER_SOURCES} 
${TCP_COMMANDS_SOURCES} PARENT_SCOPE)