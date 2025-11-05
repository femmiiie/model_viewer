#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


#include <GL/glew.h>

class ShaderLoader
{
  public:
  static GLuint Load(
    const char* vs_filepath,
		const char* tcs_filepath,
		const char* tes_filepath,
		const char* gs_filepath,
		const char* fs_filepath
  );

  static GLuint Compile(const char* filepath, GLenum type);
};


GLuint ShaderLoader::Compile(const char* filepath, GLenum type)
{
	if (!filepath) { return -1; }

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

GLuint ShaderLoader::Load(
  const char* vs_filepath,
	const char* tcs_filepath,
	const char* tes_filepath,
	const char* gs_filepath,
	const char* fs_filepath
)
{

	GLuint vsID = Compile(vs_filepath, GL_VERTEX_SHADER);
	GLuint tcsID = Compile(tcs_filepath, GL_TESS_CONTROL_SHADER);
	GLuint tesID = Compile(tes_filepath, GL_TESS_EVALUATION_SHADER);
	GLuint gsID = Compile(gs_filepath, GL_GEOMETRY_SHADER);
	GLuint fsID = Compile(fs_filepath, GL_FRAGMENT_SHADER);

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

#endif