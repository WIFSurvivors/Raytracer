#include "includes/TcpExecuter.hpp"


TcpExecuter::TcpExecuter()
{

}

int TcpExecuter::execute(TcpCommand& command) { 
    std::cout << "Executing command " << command.execute() << std::endl;
    return 0; 
}

int TcpExecuter::undo(TcpCommand& command) { return 0; }
