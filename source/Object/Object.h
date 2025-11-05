#ifndef OBJECT_H
#define OBJECT_H

#include "../ShaderLoader/ShaderLoader.h"

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Object
{
public:
  Object();
  virtual ~Object() = 0;

  virtual void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) = 0;
  virtual void update() = 0;

  glm::mat4 getModelMatrix() { return modelMatrix; }
  void setModelMatrix(glm::mat4 m) { modelMatrix = m; }
  void generateModelMatrix();

  void translate(const glm::vec3& translation);
  void rotate(float angle, const glm::vec3& axis);
  void scale(float factor);
  glm::mat4 getTransformation();

  std::vector<Object*>& getChildren_M() { return this->children; }

  std::string getName() { return this->name; }
  const char* getCName() { return this->name.c_str(); }

  glm::vec3& getPosition_M() { return this->position; }  
  glm::vec3& getRotation_M() { return this->rotation; }
  glm::vec3& getScaling_M() { return this->scaling; }

protected:
  GLuint VAO, VBO, EBO;
  GLuint shaderProgram;

  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 scaling  = glm::vec3(0.0f);

  glm::mat4 modelMatrix = glm::mat4(1.0f);
  GLsizei numIndices;

  std::string name = "New Object";
  std::vector<Object*> children;
};

#endif