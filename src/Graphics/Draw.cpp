#include <Graphics/Draw.hpp>

#include <GL/glew.h>

namespace Graphics
{
    GLenum getPrimitive(Primitive p)
    {
        switch (p)
        {
        case Primitive::Lines:     return GL_LINES;
        default: return GL_TRIANGLES;
        }
    }

    void clear(const Framebuffer& framebuffer, const Math::Vec4f& color)
    {
        framebuffer.bind();

        glClearColor(Math::x(color), Math::y(color), Math::z(color), Math::w(color));
        glClear(GL_COLOR_BUFFER_BIT);

        framebuffer.unbind();
    }
    
    void draw(const VAO& vertexArray, const Program& program, Primitive prim, std::optional<uint32_t> count)
    {
        program.use();
        vertexArray.bind();
        
        if (!vertexArray.hasIndices())
            glDrawArrays( getPrimitive(prim), 0, ( count ? *count : vertexArray.vertexCount() ) );
    }

    void draw(const Framebuffer& framebuffer, const VAO& vertexArray, const Program& program, Primitive prim)
    {
        framebuffer.bind();
        
        draw(vertexArray, program, prim);

        framebuffer.unbind();
    }
    
    void drawInstanced(const Framebuffer& framebuffer, const VAO& vertexArray, const Program& program, uint32_t count, Primitive prim)
    {
        framebuffer.bind();
        vertexArray.bind();
        program.use();

        if (!vertexArray.hasIndices())
            glDrawArraysInstanced( getPrimitive(prim), 0, vertexArray.vertexCount(), count );

        framebuffer.unbind();
    }
}