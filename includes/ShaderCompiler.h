#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <unordered_map>
#include <vector>


struct Shader {
    
  std::unordered_map<GLenum, GLuint> shaderIDs;
  GLuint programID;

  template<typename... Args>
  Shader(Args... paths){
    //GLenum shaderType
    static_assert((std::is_same_v<Args, std::pair<int, const char*>> && ...), "Failed: Arguments of ShaderCompiler not of type char*");
    glCreateShader(GL_VERTEX_SHADER);
    loadFiles(paths...);
    shaderLinker();
  }

  void activateShader(){
    glUseProgram(programID);
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
    //https://www.khronos.org/opengl/wiki/Shader_Compilation
    //
    const char* shaderChar = ShaderCode.c_str();
    GLuint shaderID = glCreateShader(ShaderPath.first);
    glShaderSource(shaderID, 1, &shaderChar, NULL);
    glCompileShader(shaderID);
    
    if(!checkCompileStatus(shaderID, ShaderPath.second)) return;
    
    shaderIDs[ShaderPath.first] = shaderID;

  }
  void shaderLinker(){
    programID = glCreateProgram();
    
    for (const auto& pair: shaderIDs){
      glad_glAttachShader(programID, pair.second);
    }

    glad_glLinkProgram(programID);
    
    //Yuck TODO better
    if(!checkProgramStatus(programID)) return;
    
    
    for (const auto& pair: shaderIDs){
      glad_glDetachShader(programID,pair.second);
      glad_glDeleteShader(pair.second);
    }

    
  
  }

  bool checkProgramStatus(GLuint programID){
    GLint success = 0;
    glad_glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success){
      GLint logSize = 0;
      glad_glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
      std::vector<GLchar> errorLog(logSize);
      glad_glGetProgramInfoLog(programID, logSize, &logSize, &errorLog[0]);

      std::cerr << "Error log: " << std::string(errorLog.begin(),errorLog.end()) << std::endl;

      glad_glDeleteProgram(programID);
      for (const auto& pair: shaderIDs){
        glad_glDeleteShader(pair.second);
      }

    }

    return success;
  }

  bool checkCompileStatus(GLuint shaderID, const char* path){
    GLint success = 0;
    glad_glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success){
      std::cout << "Shader Compilation for shader: " << path << std::endl;

      GLint logSize = 0;
      glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
      std::vector<GLchar> errorLog(logSize);
      glad_glGetShaderInfoLog(shaderID, logSize, &logSize, &errorLog[0]);
      glad_glDeleteShader(shaderID);

      std::cerr << "Error log: " << std::string(errorLog.begin(),errorLog.end()) << std::endl;
    }

    return success;


  }



};
