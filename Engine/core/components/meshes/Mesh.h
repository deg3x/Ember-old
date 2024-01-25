#pragma once

#include "utils/Types.h"
#include "core/components/Component.h"

class Light;
class Camera;
class Material;

class ENGINE_API Mesh : public Component
{
	MAKE_COMPONENT_TYPE(MESH)
	
public:
	
	std::shared_ptr<Material> material;
	
protected:
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indices;

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

protected:
	void SetupMesh();

public:
	Mesh() = delete;
	Mesh(const std::vector<VertexData>& data, const std::vector<unsigned int>& initIndices, const std::shared_ptr<Material>& initMaterial);
	virtual ~Mesh();

	void Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const;

	inline std::vector<VertexData> GetVertexData() const
	{
		return vertexData;
	}

	inline std::vector<unsigned int> GetIndices() const
	{
		return indices;
	}
protected:
	Mesh(const std::shared_ptr<Material>& initMaterial);
};