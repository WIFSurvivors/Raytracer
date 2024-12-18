add_subdirectory(Commands)

set(TCP_SERVER_HEADERS
    CommandManager.hpp
    TcpCommand.hpp
    TcpExecuter.hpp
    TcpServer.hpp
    TcpParser.hpp
    ParsedTcpCommand.hpp
    TcpCommandFactory.hpp
)

list(TRANSFORM TCP_SERVER_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(TCP_SERVER_HEADERS ${TCP_SERVER_HEADERS} 
${TCP_COMMANDS_HEADERS} PARENT_SCOPE)
