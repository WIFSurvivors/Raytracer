#include "includes/TcpExecuter.hpp"


TcpExecuter::TcpExecuter()
{

}

int TcpExecuter::execute(std::unique_ptr<TcpCommand> command) { 
    std::cout << "Executing command " << command->execute() << std::endl;
    return 0; 
}

int TcpExecuter::undo(std::unique_ptr<TcpCommand> command) { return 0; }
