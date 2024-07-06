#include <Graphics/Texture.hpp>

#include <GL/glew.h>

namespace Graphics
{

Texture::Texture(const Math::Vec2u& size) :
    _size(size)
{
    glGenTextures(1, &_handle);

    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Math::x(size), Math::y(size), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
}

Texture::~Texture()
{
    glDeleteTextures(1, &_handle);
}

uint64_t Texture::getTextureHandle() const
{
    return glGetTextureHandleARB(_handle);
}

void Texture::bind(uint32_t layer) const
{
    glBindTexture(GL_TEXTURE_2D, _handle);
}

}