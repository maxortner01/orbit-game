#pragma once

#include "Def.hpp"
#include "FlightData.hpp"

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

    void build_coord_mesh();

    void draw_ui();
    void draw_bg();
    void load_files();

    // Flight data file information
    std::vector<std::filesystem::path> avaliable_files;
    int selected;
    std::string current_dir;
    std::optional<FlightData> flight_data;

    // Camera orientation
    float _aspect;
    Math::Vec3<Math::Angle> rotation;
    float distance;
    Math::Vec3f offset;
    bool follow{false};

    // Graphics Data
    struct GraphicsData
    {
        Graphics::Program program;
        Graphics::VAO vao, indicator_lines;

        GraphicsData(Graphics::Program&& p) : program(std::move(p))
        {   }
    };

    std::optional<GraphicsData> graphics_data;
};