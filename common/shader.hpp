#ifndef SHADER_HPP
#define SHADER_HPP

GLuint CompileShader(char* filepath, GLenum type);
GLuint LoadShadersGeneric(const char* vs_filepath,
													const char* tcs_filepath,
													const char* tes_filepath,
													const char* gs_filepath,
												  const char* fs_filepath);

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
