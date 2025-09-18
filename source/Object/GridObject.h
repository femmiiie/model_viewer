#ifndef gridObject_hpp
#define gridObject_hpp

#include "Object.h"

class GridObject : public Object
{
public:
    GridObject();
    ~GridObject();

    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) override;
};

#endif
