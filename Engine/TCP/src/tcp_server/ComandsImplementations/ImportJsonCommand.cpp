#include "includes/CommandImplementations/ImportJsonCommand.hpp"
#include <iostream>
#include <fstream>
#include <string>

std::string importJsonCommand::execute(Engine * engine)
{

    //TODO: Implement this method to read the json file with quicktype generated code
    std::cout << "ImportJsonCommand executed " << _json_path <<std::endl;
    FILE *file = fopen(_json_path.c_str(), "r");
    if (!file)
    {
        std::cerr << "File not found" << std::endl;
        return "File not found";
    }
    while (!feof(file))
    {
        char c = fgetc(file);
        std::cout << c;
    }
    fclose(file);
    return "ImportJsonCommand executed";
}
int importJsonCommand::undo() { return 0; }
