#pragma once

#include "Framebuffer.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"

namespace Graphics
{
    enum class Primitive
    {
        Triangles, Lines
    };
 
    void clear(const Framebuffer& framebuffer, const Math::Vec4f& color);
    void draw(const VAO& vertexArray, const Program& program, Primitive prim = Primitive::Triangles, std::optional<uint32_t> count = std::nullopt);
    void draw(const Framebuffer& framebuffer, const VAO& vertexArray, const Program& program, Primitive prim = Primitive::Triangles);
    void drawInstanced(const Framebuffer& framebuffer, const VAO& vertexArray, const Program& program, uint32_t count, Primitive prim = Primitive::Triangles);
}