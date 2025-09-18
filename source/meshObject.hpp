#ifndef meshObject_hpp
#define meshObject_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>
#include <map>
#include <string>
#include "structs.h"

class meshObject {
public:
    meshObject(std::string filepath);
    ~meshObject();

    void draw(const glm::mat4& view, const glm::mat4& projection, glm::mat4 transform, light light1, light light2, glm::vec3 camera, int selected);
    void drawPicking(const glm::mat4& view, const glm::mat4& projection, glm::mat4 transform);
    void translate(const glm::vec3& translation); // Translate the object
    void rotate(float angle, const glm::vec3& axis); // Rotate the object

    int getId() const { return id; } // Getter for the ID
    void setModelMatrix(glm::mat4 m) { modelMatrix = m; }
    glm::mat4 getModelMatrix() { return modelMatrix; }
    glm::mat4 getTransformation();
    
    static meshObject* getMeshObjectById(int id); // Retrieve object by ID
    
    std::vector<meshObject*> children;

private:
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    GLuint pickingShaderProgram;
    glm::mat4 modelMatrix;

    static int nextId; // Static counter for unique IDs
    int id;            // ID for this specific object
    
    GLsizei numIndices;
    
    static std::map<int, meshObject*> meshObjectMap; // Static map of ID to Object
};

#endif
