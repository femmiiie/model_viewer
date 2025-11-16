#include "MeshObject.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

MeshObject::MeshObject(std::vector<LightObject*>& scene_lights, std::string filepath)
    : Object(), scene_lights(scene_lights)
{
	using namespace tinyobj;

	ObjReader reader;
	if (!reader.ParseFromFile(filepath))
	{
		valid = false;
		return;
	}

	attrib_t attrib                   = reader.GetAttrib();
	std::vector<shape_t> shapes       = reader.GetShapes();
	std::vector<material_t> materials = reader.GetMaterials();
	materials.push_back(material_t());

	name = !shapes[0].name.empty() ? shapes[0].name : name;

	std::vector<GLuint> indices;
	std::vector<glm::vec3> verts  = Loader::loadVertices(attrib, shapes, indices);
	std::vector<glm::vec2> tex    = Loader::loadTextures(attrib, shapes);
	std::vector<glm::vec3> colors = Loader::loadColors(attrib, shapes);
	std::vector<glm::vec3> norms  = Loader::loadNormals(attrib, shapes, verts);

	glBindVertexArray(VAO);

	numIndices = indices.size();

	glGenBuffers(1, &normal_buffer);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),
	             GL_STATIC_DRAW);

	std::cout << "here" << std::endl;

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), norms.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	if (!tex.empty())
	{
		glGenBuffers(1, &texture_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
		glBufferData(GL_ARRAY_BUFFER, tex.size() * sizeof(glm::vec2), tex.data(), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(2);

	if (!tex.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(3);
	}

	glBindVertexArray(0);

	// Load and compile shaders
	shaderProgram = ShaderLoader::Load("mesh.vs.glsl", NULL, NULL, NULL, "mesh.fs.glsl");
}

MeshObject::~MeshObject()
{
	glDeleteBuffers(1, &normal_buffer);
	glDeleteBuffers(1, &texture_buffer);
}

void MeshObject::draw(const glm::mat4& view, const glm::mat4& projection,
                      const glm::mat4& transform, const glm::vec3& camera)
{
	glUseProgram(shaderProgram);

	glm::mat4 M = transform * modelMatrix;
	GLuint MID  = glGetUniformLocation(shaderProgram, "M");
	glUniformMatrix4fv(MID, 1, GL_FALSE, glm::value_ptr(M));

	GLuint VID = glGetUniformLocation(shaderProgram, "V");
	glUniformMatrix4fv(VID, 1, GL_FALSE, glm::value_ptr(view));

	GLuint PID = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(PID, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform1i(glGetUniformLocation(shaderProgram, "num_lights"), this->scene_lights.size());

	GLuint cameraID = glGetUniformLocation(shaderProgram, "camerapos");
	glUniform3fv(cameraID, 1, glm::value_ptr(camera));

	// Draw the object
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (Object* child : this->children)
	{
		child->draw(view, projection, M, camera);
	}
}

std::vector<glm::vec3> MeshObject::Loader::loadVertices(const tinyobj::attrib_t& attrib,
                                                        const std::vector<tinyobj::shape_t>& shapes,
                                                        std::vector<GLuint>& outIndices)
{
	size_t index_count = 0;
	for (const auto& shape : shapes)
	{
		index_count += shape.mesh.indices.size();
	}

	std::vector<glm::vec3> vertices;
	vertices.reserve(index_count);
	outIndices.reserve(index_count);

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			vertices.emplace_back(attrib.vertices[(3 * index.vertex_index) + 0],
			                      attrib.vertices[(3 * index.vertex_index) + 1],
			                      attrib.vertices[(3 * index.vertex_index) + 2]);

			outIndices.emplace_back(static_cast<GLuint>(outIndices.size()));
		}
	}

	return vertices;
}

std::vector<glm::vec2> MeshObject::Loader::loadTextures(const tinyobj::attrib_t& attrib,
                                                        const std::vector<tinyobj::shape_t>& shapes)
{
	if (attrib.texcoords.empty())
	{
		return {};
	}

	size_t index_count = 0;
	for (const auto& shape : shapes)
	{
		index_count += shape.mesh.indices.size();
	}

	std::vector<glm::vec2> tex;
	tex.reserve(index_count);

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			if (index.texcoord_index < 0)
			{
				tex.emplace_back(0.0f, 0.0f);
				continue;
			}

			size_t base = (2 * index.texcoord_index);
			if (base + 1 >= attrib.texcoords.size())
			{
				tex.emplace_back(0.0f, 0.0f);
				continue;
			}

			tex.emplace_back(attrib.texcoords[base + 0], attrib.texcoords[base + 1]);
		}
	}

	return tex;
}

std::vector<glm::vec3> MeshObject::Loader::loadColors(const tinyobj::attrib_t& attrib,
                                                      const std::vector<tinyobj::shape_t>& shapes)
{
	size_t index_count = 0;
	for (const auto& shape : shapes)
	{
		index_count += shape.mesh.indices.size();
	}

	std::vector<glm::vec3> colors = std::vector<glm::vec3>(index_count / 3, glm::vec3(1.0f));
	colors.reserve(index_count);

	if (!attrib.colors.empty())
	{
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				colors.emplace_back(attrib.colors[3 * index.vertex_index + 0],
				                    attrib.colors[3 * index.vertex_index + 1],
				                    attrib.colors[3 * index.vertex_index + 2]);
			}
		}
	}

	return colors;
}

std::vector<glm::vec3> MeshObject::Loader::loadNormals(const tinyobj::attrib_t& attrib,
                                                       const std::vector<tinyobj::shape_t>& shapes,
                                                       const std::vector<glm::vec3>& verts)
{
	bool requiresGeneration = attrib.normals.empty();
	if (!requiresGeneration)
	{
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				if (index.normal_index < 0)
				{
					requiresGeneration = true;
					break;
				}
			}
			if (requiresGeneration)
			{
				break;
			}
		}
	}

	if (requiresGeneration)
	{
		return generateNormals(shapes, verts);
	}

	std::vector<glm::vec3> normals;
	normals.reserve(verts.size());

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			size_t normalBase = static_cast<size_t>(3 * index.normal_index);
			if (normalBase + 2 >= attrib.normals.size())
			{
				return generateNormals(shapes, verts);
			}

			normals.emplace_back(attrib.normals[normalBase + 0], attrib.normals[normalBase + 1],
			                     attrib.normals[normalBase + 2]);
		}
	}

	return normals;
}

std::vector<glm::vec3>
MeshObject::Loader::generateNormals(const std::vector<tinyobj::shape_t>& shapes,
                                    const std::vector<glm::vec3>& verts)
{
	std::vector<glm::vec3> normals(verts.size(), glm::vec3(0.0f));
	if (verts.empty())
	{
		return normals;
	}

	size_t cursor = 0;
	const glm::vec3 defaultNormal(0.0f, 1.0f, 0.0f);

	for (const auto& shape : shapes)
	{
		const auto& faceVertices = shape.mesh.num_face_vertices;
		for (size_t f = 0; f < faceVertices.size(); ++f)
		{
			if (cursor >= verts.size())
			{
				return normals;
			}

			int fv = faceVertices[f];
			if (fv <= 0)
			{
				continue;
			}

			if (cursor + static_cast<size_t>(fv) > verts.size())
			{
				fv = static_cast<int>(verts.size() - cursor);
			}

			size_t faceStart = cursor;
			glm::vec3 faceNormal(0.0f);

			if (fv >= 3)
			{
				const glm::vec3& v0 = verts[faceStart];
				for (int v = 1; v < fv - 1; ++v)
				{
					const glm::vec3& v1 = verts[faceStart + v];
					const glm::vec3& v2 = verts[faceStart + v + 1];
					glm::vec3 triNormal = glm::cross(v1 - v0, v2 - v0);
					float length        = glm::length(triNormal);
					if (length > 0.0f)
					{
						faceNormal += triNormal / length;
					}
				}
			}

			if (glm::length(faceNormal) == 0.0f)
			{
				faceNormal = defaultNormal;
			}
			else
			{
				faceNormal = glm::normalize(faceNormal);
			}

			for (int v = 0; v < fv; ++v)
			{
				normals[faceStart + static_cast<size_t>(v)] = faceNormal;
			}

			cursor += static_cast<size_t>(fv);
		}
	}

	return normals;
}