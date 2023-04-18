#pragma once

#include "../Component.h"
#include "../../../utils/Types.h"

#include <vector>
#include <memory>

class Material;

class Mesh : public Component
{
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
	Mesh(std::vector<VertexData> data, std::vector<unsigned int> initIndices, const std::shared_ptr<Material>& initMaterial);
	virtual ~Mesh();

	void Draw() const;

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