#include "includes/CommandImplementations/GetRootCommand.hpp"
#include "includes/Entity.hpp"
GetRootCommand::GetRootCommand() 
{
    std::cout << "GetRootCommand::GetRootCommand()" << std::endl;
   
}

int GetRootCommand::undo() 
{
    std::cout << "GetRootCommand::undo()" << std::endl;
    return 0;
}

int GetRootCommand::execute(Engine * engine) 
{
    std::cout << "GetRootCommand::execute()" << std::endl;
    engine->get_scene()->get_root().lock()->print();
    return 0;
}


