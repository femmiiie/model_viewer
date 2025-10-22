#ifndef SHADER_HPP
#define SHADER_HPP

GLuint CompileShader(char* filepath, GLenum type);
GLuint LoadShadersGeneric(char* vs_filepath,
													char* tcs_filepath,
													char* tes_filepath,
													char* gs_filepath,
												  char* fs_filepath);

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
