#include "LightObject.h"

#include <glm/gtx/string_cast.hpp>


LightObject::LightObject() : Object()
{    
  name = "New Light";

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  std::vector<GLfloat> vertex = { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f };
  glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);

  // Vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  shaderProgram = ShaderLoader::Load(
    "grid.vs.glsl",
    NULL,
    NULL,
    NULL,
    "point.fs.glsl"
  );
}


void LightObject::draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) 
{
    glUseProgram(shaderProgram);
 
    std::cout << glm::to_string(modelMatrix) << std::endl;

    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    glBindVertexArray(VAO);
    glPointSize(10 + this->power/3); //TODO: CHANGE TO CUSTOM SETTING

    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);

    for (Object* child : this->children)
    {
        child->draw(view, projection, transform * modelMatrix, camera);
    }
}


void LightObject::update()
{
  generateModelMatrix();

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  std::vector<GLfloat> vertex = {position.x, position.y, position.z, color.x, color.y, color.z};
  glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), vertex.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);

  std::cout << "updated" << std::endl;

}


void LightObject::setLightingData(LightData& data)
{
  
}