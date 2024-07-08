#pragma once

#include "../Def.hpp"

#include <vector>

namespace Graphics
{
    struct Buffer
    {
        enum Target
        {
            Vertex, Index, ShaderStorage
        };

        Buffer(Target target);

        Buffer(Buffer&&) = delete;
        Buffer(const Buffer&) = delete;

        ~Buffer();

        void bind(uint32_t base = 0) const;

        template<typename T>
        std::vector<T> getData() const
        {
            ASSERT( _size % sizeof(T) == 0, "Type incompatibility" );
            std::vector<T> vec(_size / sizeof(T));
            getData(&*vec.begin());
            return vec;
        }

        void getData(void* ptr) const; // performs copy

        // maybe have begin, end iterator
        void setData(const void* data, std::size_t bytesize);

        template<typename T>
        void setData(const std::vector<T>& data)
        {
            setData(&data[0], data.size() * sizeof(T));
        }

        auto getSize() const { return _size; }

    private:
        uint32_t _handle;
        std::size_t _size;
        Target _target;
        void* mapped_ptr;
    };

    struct VAO
    {
        VAO();
        ~VAO();

        VAO(VAO&& v) :
            vertices(std::move(v.vertices)),
            indices(std::move(v.indices)),
            _handle([](auto& handle){ const auto val = handle; handle = 0; return val; }(v._handle)),
            _vertex_count(v._vertex_count)
        {   }

        bool hasVertices() const { return (indices ? true : false); }
        const auto& getVertexBuffer()
        {
            if (!vertices) 
                vertices = std::make_unique<Graphics::Buffer>(Graphics::Buffer::Vertex);
            return vertices;
        }

        bool hasIndices() const { return (indices ? true : false); }
        const auto& getIndexBuffer()
        {
            if (!indices) 
                indices = std::make_unique<Graphics::Buffer>(Graphics::Buffer::Index);
            return indices;
        }

        void bind() const;

        void addVertexAttribute(uint32_t location, uint32_t element_count, std::size_t total_size, std::size_t offset);

        auto vertexCount() const { return _vertex_count; }

    private:
        std::unique_ptr<Graphics::Buffer> vertices, indices;
        uint32_t _handle, _vertex_count;
    };
}