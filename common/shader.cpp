#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.hpp"


GLuint CompileShader(char* filepath, GLenum type)
{
	if (filepath == NULL) { return -1; }

	GLuint ShaderID = glCreateShader(type);

	//Read Shader
	std::string ShaderCode;
	std::ifstream ShaderStream(filepath, std::ios::in);
	if (ShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << ShaderStream.rdbuf();
		ShaderCode = sstr.str();
		ShaderStream.close();
	}
	else
	{
		std::cout << "Impossible to open " << filepath << ". Are you in the right directory?" << std::endl;
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Compile Shader
	printf("Compiling shader : %s\n", filepath);
	const char* SourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &SourcePointer , NULL);
	glCompileShader(ShaderID);

	// Check Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}

	return ShaderID;
}

GLuint LoadShadersGeneric(char* vs_filepath,
													char* tcs_filepath,
													char* tes_filepath,
													char* gs_filepath,
												  char* fs_filepath)
{

	GLuint vsID = CompileShader(vs_filepath, GL_VERTEX_SHADER);
	GLuint tcsID = CompileShader(tcs_filepath, GL_TESS_CONTROL_SHADER);
	GLuint tesID = CompileShader(tes_filepath, GL_TESS_EVALUATION_SHADER);
	GLuint gsID = CompileShader(gs_filepath, GL_GEOMETRY_SHADER);
	GLuint fsID = CompileShader(fs_filepath, GL_FRAGMENT_SHADER);

	if (!vsID || !tcsID || !tesID || !gsID || !fsID ) { return 0; }

	printf("Linking program\n");
	GLuint programID = glCreateProgram();

	if (vsID != -1) { glAttachShader(programID, vsID); }
	if (tcsID != -1) { glAttachShader(programID, tcsID); }
	if (tesID != -1) { glAttachShader(programID, tesID); }
	if (gsID != -1) { glAttachShader(programID, gsID); }
	if (fsID != -1) { glAttachShader(programID, fsID); }

	glLinkProgram(programID);

	if (vsID != -1)
	{
		glDetachShader(programID, vsID);
		glDeleteShader(vsID);
	}
	
	if (tcsID != -1)
	{
		glDetachShader(programID, tcsID);
		glDeleteShader(tcsID);
	}

	if (tesID != -1)
	{
		glDetachShader(programID, tesID);
		glDeleteShader(tesID);
	}

	if (gsID != -1)
	{
		glDetachShader(programID, gsID);
		glDeleteShader(gsID);
	}

	if (fsID != -1)
	{
		glDetachShader(programID, fsID);
		glDeleteShader(fsID);
	}
	return programID;
}


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


