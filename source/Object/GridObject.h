#ifndef GRIDOBJECT_H
#define GRIDOBJECT_H

#include "Object.h"

class GridObject : public Object
{
public:
    GridObject(std::vector<Light>& scene_lights);
    ~GridObject();

    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);
};

#endif
