#include "includes/TcpExecuter.hpp"


TcpExecuter::TcpExecuter()
{

}

const std::string& TcpExecuter::execute(TcpCommand *command, Engine *engine) { 
    std::string return_value = command->execute(engine);
    std::cout << "Executing command " << return_value << std::endl;
    return return_value; 
}

const std::string& TcpExecuter::undo(TcpCommand *command) { return "0"; }
