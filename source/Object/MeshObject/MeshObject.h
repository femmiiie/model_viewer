#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "../Object.h"
#include "../LightObject/LightData.h"

class MeshObject : public Object
{
public:
    MeshObject(std::vector<LightData*>& scene_lights, std::string filepath);
    ~MeshObject();

    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);
    void update()
    {
        generateModelMatrix();
    }

private:
    std::vector<LightData*>& scene_lights;
};

#endif
