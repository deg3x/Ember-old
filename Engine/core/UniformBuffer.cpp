#include "engine_pch.h"
#include "UniformBuffer.h"

#include <glad/glad.h>

#include "Shader.h"
#include "logger/Logger.h"

UniformBuffer::UniformBuffer(uint32_t size)
{
    glGenBuffers(1, &id);

    Bind();
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    Unbind();
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &id);
}

void UniformBuffer::Bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, id);
}

void UniformBuffer::Unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetData(uint32_t offset, uint32_t size, const void* data) const
{
    Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    Unbind();
}

void UniformBuffer::SetBinding(uint32_t bindingID) const
{
    Bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingID, id);
    Unbind();
}

void UniformBuffer::SetBindingForShader(const std::shared_ptr<Shader>& shader, const char* blockName, uint32_t bindingID) const
{
    const unsigned int blockID = glGetUniformBlockIndex(shader->GetShaderID(), blockName);
    if (blockID == GL_INVALID_INDEX)
    {
        Logger::Log(LogCategory::ERROR, "Invalid attempt to set uniform block binding for shader", "UniformBuffer::SetBindingForShader");
        return;
    }

    glUniformBlockBinding(shader->GetShaderID(), blockID, bindingID);
    SetBinding(bindingID);
}
