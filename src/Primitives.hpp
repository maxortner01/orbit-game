#pragma once

#include "Graphics/Buffer.hpp"
#include "Math.hpp"

namespace Orbit
{
    struct Vertex
    {
        Math::Vec3f position, normal;
        Math::Vec4f color;
    };

    struct Primitive
    {
        static Graphics::VAO createSphere(float r, int32_t res);
    };
}