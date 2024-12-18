#include "includes/tcp_server/TcpExecuter.hpp"


TcpExecuter::TcpExecuter()
{

}

std::string TcpExecuter::execute(TcpCommand *command, Engine *engine) { 
    std::string return_value = command->execute(engine);
    std::cout << "Returning to client: " << return_value << std::endl;
    return return_value; 
}

int TcpExecuter::undo(TcpCommand *command) { return 0; }
