#include "includes/CommandImplementations/GetRootCommand.hpp"

GetRootCommand::GetRootCommand() 
{
    std::cout << "GetRootCommand::GetRootCommand()" << std::endl;
   
}

int GetRootCommand::undo() 
{
    std::cout << "GetRootCommand::undo()" << std::endl;
    return 0;
}

int GetRootCommand::execute() 
{
    std::cout << "GetRootCommand::execute()" << std::endl;
    return 0;
}


