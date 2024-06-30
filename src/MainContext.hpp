#pragma once

#include "Def.hpp"
#include "FlightData.hpp"

#include <optional>
#include <SDL3/SDL.h>

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
    void draw_bg();

    std::optional<FlightData> flight_data;
};