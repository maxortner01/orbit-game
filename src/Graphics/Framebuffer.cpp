#include <Graphics/Framebuffer.hpp>

#include <GL/glew.h>

namespace Graphics
{

Framebuffer::Framebuffer(const Math::Vec2u& size) :
    color(std::make_unique<Texture>(size))
{
    glGenFramebuffers(1, &_handle);

    bind();
    color->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->getHandle(), 0);
    ASSERT(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete");
    unbind();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &_handle);
}

void Framebuffer::rebuild(const Math::Vec2u& size)
{
    color.reset();
    color = std::make_unique<Texture>(size);
    bind();
    color->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->getHandle(), 0);
    ASSERT(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete");
    unbind();
}

void Framebuffer::bind() const
{
    ASSERT(_handle, "Invalid framebuffer");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
    glViewport(0, 0, Math::x(color->getSize()), Math::y(color->getSize()));
}

void Framebuffer::unbind() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

}