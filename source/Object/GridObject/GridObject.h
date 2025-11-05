#ifndef GRIDOBJECT_H
#define GRIDOBJECT_H

#include "../Object.h"

class GridObject : public Object
{
public:
    GridObject();
    ~GridObject();

    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);
    void update() { generateModelMatrix(); }

    int& getGridSize_M() { return this->gridSize; }
    int& getGridSpacing_M() { return this->gridSpacing; }
    
    void generateGrid();

private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    int gridSize;
    int gridSpacing;
};

#endif
