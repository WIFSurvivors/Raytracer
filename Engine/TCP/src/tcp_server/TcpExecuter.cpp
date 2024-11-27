#include "includes/TcpExecuter.hpp"


TcpExecuter::TcpExecuter()
{

}

int TcpExecuter::execute(TcpCommand *command, Engine *engine) { 
    std::cout << "Executing command " << command->execute(engine) << std::endl;
    return 0; 
}

int TcpExecuter::undo(TcpCommand *command) { return 0; }
