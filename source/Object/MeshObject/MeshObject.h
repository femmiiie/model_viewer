#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "../LightObject/LightData.h"
#include "../Object.h"
#include <tiny_obj_loader.h>

using namespace tinyobj;

class MeshObject : public Object
{
 public:
 MeshObject(std::vector<LightObject *> &scene_lights, std::string filepath);
 ~MeshObject();

 void draw(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &transform,
           const glm::vec3 &camera);
 void update() { generateModelMatrix(); }

 bool isValid() { return this->valid; }

 private:
 GLuint normal_buffer  = 0;
 GLuint texture_buffer = 0;
 GLuint color_buffer   = 0;

 std::vector<LightObject *> &scene_lights;

 bool valid = true;

 void initializeDefaultMaterial();

 class Loader
 {
public:
	static std::vector<glm::vec3> loadVertices(const attrib_t &attrib,
	                                           const std::vector<shape_t> &shapes,
	                                           std::vector<GLuint> &outIndices);
	static std::vector<glm::vec3> loadNormals(const attrib_t &attrib,
	                                          const std::vector<shape_t> &shapes,
	                                          const std::vector<glm::vec3> &verts);
	static std::vector<glm::vec3> loadColors(const attrib_t &attrib,
	                                         const std::vector<shape_t> &shapes);
	static std::vector<glm::vec2> loadTextures(const attrib_t &attrib,
	                                           const std::vector<shape_t> &shapes);

private:
	static std::vector<glm::vec3> generateNormals(const std::vector<shape_t> &shapes,
	                                              const std::vector<glm::vec3> &verts);
 };
};

#endif
