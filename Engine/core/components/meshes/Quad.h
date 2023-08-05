#pragma once

#include "Mesh.h"

class ENGINE_API Quad : public Mesh
{
public:
    Quad(const std::shared_ptr<Material>& initMaterial);
    virtual ~Quad() override = default;
    
    void GenerateVertexData();
    void GenerateIndices();
};
