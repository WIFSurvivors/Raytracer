#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <unordered_map>


struct Shader {
    
  std::unordered_map<GLenum, unsigned int> shaderIDs;

  template<typename... Args>
  Shader(Args... paths){
    //GLenum shaderType
    static_assert((std::is_same_v<Args, std::pair<GLenum, const char*>> && ...), "Failed: Arguments of ShaderCompiler not of type char*");
    glCreateShader(GL_VERTEX_SHADER);
    loadFiles(paths...);
  }
  
  private:
  
  template<typename... Args>
  void loadFiles(Args... paths){
    (... , loadFile(paths));
  }

  void loadFile(std::pair<GLenum,const char*> ShaderPath){

    std::string ShaderCode;
    std::ifstream ShaderFile;
    ShaderFile.exceptions(std::ios::failbit | std::ios::badbit);
    
    //Try Open file Read content and close afterwards
    try{
      ShaderFile.open(ShaderPath.second);
      std::stringstream shaderstream; 
      shaderstream << ShaderFile.rdbuf();
      ShaderCode = shaderstream.str();
      ShaderFile.close();
    }catch(std::exception& e){
      std::cerr << e.what() << std::endl;
    }

    //Content of File should now be loaded succesfully
    //

    
    

    




  }



};
