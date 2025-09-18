#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>
#include <map>
#include <string>
#include "structs.h"
#include "Object.h"

class MeshObject : public Object {
public:
    MeshObject(std::string filepath, std::vector<Renderer::Light>& scene_lights);
    ~MeshObject();

    void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) override;
};

#endif
