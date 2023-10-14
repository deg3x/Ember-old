#pragma once
#include "Object.h"

class ENGINE_API Skybox : public Object
{
public:
    Skybox();
    virtual ~Skybox();

    void Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const override;
};
