#pragma once

#include "Object.h"

class ENGINE_API Skybox : public Object
{
public:
    Skybox(const char* objName);
    virtual ~Skybox() {}

    void Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const override;
};
