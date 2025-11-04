#ifndef OBJECT_H
#define OBJECT_H

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

  glm::mat4 getModelMatrix() { return modelMatrix; }
  void setModelMatrix(glm::mat4 m) { modelMatrix = m; }

  void translate(const glm::vec3& translation);
  void rotate(float angle, const glm::vec3& axis);
  void scale(float factor);
  glm::mat4 getTransformation();

  std::vector<Object*>& getChildren_M() { return this->children; }

  std::string getName() { return this->name; }
  const char* getCName() { return this->name.c_str(); }

protected:
  GLuint VAO, VBO, EBO;
  GLuint shaderProgram;
  glm::mat4 modelMatrix;
  GLsizei numIndices;

  std::string name = "New Object";
  std::vector<Object*> children;
};

#endif