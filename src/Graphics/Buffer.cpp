#include <Graphics/Buffer.hpp>

#include <GL/glew.h>

namespace Graphics
{

GLenum getTarget(Buffer::Target target)
{
    switch (target)
    {
    case Buffer::Target::Vertex:        return GL_ARRAY_BUFFER;
    case Buffer::Target::Index:         return GL_ELEMENT_ARRAY_BUFFER;
    case Buffer::Target::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
    default: return 0;
    }
}

Buffer::Buffer(Target target) :
    _handle(0), _size(0), _target(target)
{   
    glGenBuffers(1, &_handle);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &_handle);
}

void Buffer::bind(uint32_t base) const
{
    ASSERT(_handle, "invalid buffer");
    glBindBuffer(getTarget(_target), _handle);
    if (_target == Target::ShaderStorage)
        glBindBufferBase(getTarget(_target), base, _handle);
}

void Buffer::getData(void* ptr) const
{
    if (!_size) return;

    bind();
    const auto* mapped = glMapBufferRange(getTarget(_target), 0, _size, GL_MAP_READ_BIT);
    std::memcpy(ptr, mapped, _size);
    glUnmapBuffer(getTarget(_target));
}

void Buffer::setData(const void* data, std::size_t bytesize)
{
    if (!bytesize) return;

    bind();
    if (_size != bytesize)
        glBufferData(getTarget(_target), bytesize, data, GL_STATIC_DRAW); // should make this an option
    else
        glBufferSubData(getTarget(_target), 0, bytesize, data);
    _size = bytesize;
}

VAO::VAO() :
    _vertex_count{0}
{
    glGenVertexArrays(1, &_handle);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &_handle);
}

void VAO::bind() const
{
    glBindVertexArray(_handle);
    if (vertices) vertices->bind();
    if (indices) indices->bind();
}

void VAO::addVertexAttribute(uint32_t location, uint32_t element_count, std::size_t total_size, std::size_t offset)
{
    ASSERT(vertices, "Attempting to add attribute to missing vertex buffer");
    ASSERT(vertices->getSize() % total_size == 0, "Invalid vertex size! vertices->getSize() = " << vertices->getSize() << ", total_size = " << total_size);

    bind();
    vertices->bind();
    glVertexAttribPointer(location, element_count, GL_FLOAT, GL_FALSE, total_size, (void*)offset);
    glEnableVertexAttribArray(location);

    _vertex_count = vertices->getSize() / total_size;
}

}