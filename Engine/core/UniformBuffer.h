#pragma once

#include "Definitions.h"

class Shader;

class ENGINE_API UniformBuffer
{
private:
    unsigned int id;
    
public:
    UniformBuffer() = delete;
    UniformBuffer(uint32_t size);
    virtual ~UniformBuffer();

    void Bind() const;
    void Unbind()const;

    void SetData(uint32_t offset, uint32_t size, const void* data) const;
    void SetBinding(uint32_t bindingID) const;
    void SetBindingForShader(const std::shared_ptr<Shader>& shader, const char* blockName, uint32_t bindingID) const;
};