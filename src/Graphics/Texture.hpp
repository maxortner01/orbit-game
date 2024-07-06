#pragma once

#include "../Math.hpp"

namespace Graphics
{
    struct Texture
    {
        Texture(const Math::Vec2u& size); // create empty texture
        // Texture(const std::string& file) // from file

        Texture(Texture&&) = delete;
        Texture(const Texture&) = delete;

        ~Texture();

        uint64_t getTextureHandle() const;

        void bind(uint32_t layer = 0) const;

        auto getHandle() const { return _handle; }
        auto getSize() const { return _size; }
        auto aspectRatio() const { return (float)Math::x(_size) / (float)Math::y(_size); }

    private:
        uint32_t _handle;
        Math::Vec2u _size;
    };
}