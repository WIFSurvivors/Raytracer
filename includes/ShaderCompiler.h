#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <unordered_map>
#include <vector>


struct Shader {
    
  std::unordered_map<GLenum, GLuint> shaderIDs;

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
    //https://www.khronos.org/opengl/wiki/Shader_Compilation
    //
    const char* shaderChar = ShaderCode.c_str();
    GLuint shaderID = glCreateShader(ShaderPath.first);
    glShaderSource(shaderID, 1, &shaderChar, NULL);
    glCompileShader(shaderID);
    
    if(!checkCompileStatus(shaderID, ShaderPath.second)) return;
    
    GLuint programID = glCreateProgram();
    glad_glAttachShader(programID, shaderID);
    glad_glLinkProgram(programID);
    
    //Yuck TODO better
    if(!checkProgramStatus(programID,shaderID)) return;

    glad_glDetachShader(programID,shaderID);
    glad_glDeleteShader(shaderID);

    shaderIDs[ShaderPath.first] = programID;

    
  }

  bool checkProgramStatus(GLuint programID, GLuint shaderID){
    GLint success = 0;
    glad_glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success){
      GLint logSize = 0;
      glad_glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
      std::vector<GLchar> errorLog(logSize);
      glad_glGetProgramInfoLog(programID, logSize, &logSize, &errorLog[0]);

      std::cerr << "Error log: " << std::string(errorLog.begin(),errorLog.end()) << std::endl;

      glad_glDeleteProgram(programID);
      glad_glDeleteShader(shaderID);

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