#pragma once

#include "../Def.hpp"

#include "Texture.hpp"

namespace Graphics
{
    struct Framebuffer
    {
        Framebuffer(const Math::Vec2u& size);

        Framebuffer(Framebuffer&&) = delete;
        Framebuffer(const Framebuffer&) = delete;

        ~Framebuffer();

        void rebuild(const Math::Vec2u& size);

        void bind() const;
        void unbind() const;

        auto getSize() const { return color->getSize(); }

        const auto& getColor() const { return color; }

    private:
        uint32_t _handle;
        std::unique_ptr<Texture> color;
    };
}