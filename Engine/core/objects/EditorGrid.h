#pragma once

#include "Object.h"

class ENGINE_API EditorGrid : public Object
{
public:
    EditorGrid();
    virtual ~EditorGrid() {}
    
    void Draw(const std::shared_ptr<Camera>& camera, const std::vector<std::shared_ptr<Light>>& lights) const override;
};
