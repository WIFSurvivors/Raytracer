add_subdirectory(CommandImplementations)
add_subdirectory(utility)

set(TCP_SERVER_HEADERS
    CommandManager.hpp
    GetRootCommand.hpp
    TcpCommand.hpp
    TcpExecuter.hpp
    TcpServer.hpp
    TcpParser.hpp
    ParsedTcpCommand.hpp
    TcpCommandFactory.hpp
    ${TCP_COMMANDS_HEADERS}
    ${UTILITY_HEADERS}
)

list(TRANSFORM TCP_SERVER_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(TCP_SERVER_HEADERS ${TCP_SERVER_HEADERS} PARENT_SCOPE)