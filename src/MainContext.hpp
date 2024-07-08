#pragma once

#include "Def.hpp"

#include "Graphics/Buffer.hpp"
#include "Graphics/Shader.hpp"

#include <filesystem>
#include <vector>
#include <optional>
#include <SDL3/SDL.h>

struct Vertex
{
    Math::Vec3f position;
    Math::Vec4f color;
};

struct MainContext
{
    MainContext();
    ~MainContext();

    MainContext(const MainContext&) = delete;

    void clear();
    void draw();
    void pollEvents();

    bool open() const { return _open; }

private:
    SDL_Window* window;
    SDL_GLContext context;
    bool _open;

    void draw_ui();
    void update(double dt);
    void draw_bg();

    // Camera orientation
    float _aspect;
    Math::Vec3<Math::Angle> rotation;
    float distance;
    Math::Vec3f offset;

    struct GraphicsData
    {
        Graphics::Program program;

        GraphicsData(Graphics::Program&& p) :
            program(std::move(p))
        {   }
    };

    struct Planet
    {
        // Air density
        float p,k,p_0;
        float getAirDensity(float altitude)
        {
            return p_0 * ( altitude > p ? 0.f : powf( 1.0 - (1.0 / p) * std::max<float>(altitude, 0.f), k ) );
        }

        float r;
        Math::Vec3f position;
        Math::Vec3f velocity;
    } planet;

    struct Object
    {
        float r;
        Math::Vec3f position, velocity, thrust;
    } object;

    std::optional<GraphicsData> graphics_data;
    std::optional<Graphics::VAO> sphere;
};