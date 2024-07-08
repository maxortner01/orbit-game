#include "MainContext.hpp"

#include "Graphics/Draw.hpp"
#include "Primitives.hpp"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>
#include <GL/glew.h>
#include <Math.hpp>
#include <sstream>
#include <chrono>

#ifndef RES_DIR
#define RES_DIR "."
#endif

MainContext::MainContext() :
    distance{1}
{
    ASSERT(!SDL_Init(SDL_INIT_VIDEO), "Error initializing SDL");

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );

    window = SDL_CreateWindow("Hello", 1280U, 720U, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext( window );

    glewExperimental = GL_TRUE;
    ASSERT(glewInit() == GLEW_OK, "Error initializing OpenGL");

    _aspect = 1280.f / 720.f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();

    _open = true;

    graphics_data.emplace(Graphics::ProgramBuilder()
        .addShader(RES_DIR "/shaders/vertex.glsl",   Graphics::Shader::Vertex  )
        .addShader(RES_DIR "/shaders/fragment.glsl", Graphics::Shader::Fragment)
        .build());

    sphere.emplace(Orbit::Primitive::createSphere(1.f, 20));

    planet.r = 1500.f;
    planet.p = 50.f;
    planet.k = 12.f;
    planet.p_0 = 10.f;

    object.r = 0.05f;
    object.position = { planet.r + object.r, 0.f, 0.f };
}

MainContext::~MainContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

void MainContext::clear()
{
    glClearColor(0.05f, 0.1f, 0.15f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainContext::draw()
{
    static auto start = std::chrono::high_resolution_clock::now();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    glDisable(GL_DEPTH_TEST);

    const auto now = std::chrono::high_resolution_clock::now();
    const auto dt = std::chrono::duration<double, std::chrono::seconds::period>(start - now);
    start = now;
    update(dt.count());
    
    draw_ui();

    clear();
    ImGui::Render();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    draw_bg();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void MainContext::pollEvents()
{
    static bool left_down = false;

    bool check_mouse = true;
    bool check_keyboard = true;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (ImGui::GetIO().WantCaptureMouse) { left_down = false; check_mouse = false; }
        if (ImGui::GetIO().WantCaptureKeyboard) { check_keyboard = false; }

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: _open = false; break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT && check_mouse)
                left_down = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT && check_mouse)
                left_down = false;
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            if (check_mouse)
                distance -= distance * event.wheel.y * 0.2f;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            if (left_down && check_mouse)
            {
                Math::y(rotation) -= Math::Angle::degrees(event.motion.xrel * 0.2f);
                Math::x(rotation) -= Math::Angle::degrees(event.motion.yrel * 0.2f);
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_SPACE && check_keyboard)
                object.thrust = Math::Vec3f{ 0.2f, 0.f, 0.f };
            if (event.key.key == SDLK_R)
                object.thrust = Math::Vec3f{ 0.f, 0.f, 0.2f };
            break;

        case SDL_EVENT_KEY_UP:
            if (check_keyboard)
            {
                if (event.key.key == SDLK_SPACE)
                    object.thrust = Math::Vec3f{ 0.f, 0.f, 0.f };
                if (event.key.key == SDLK_R)
                    object.thrust = Math::Vec3f{ 0.f, 0.f, 0.f };
            }
            break;
        default: break;
        }
    }
}

void MainContext::update(double dt)
{
    const auto r = planet.position - object.position;
    object.velocity += (
        Math::normalized(r) * 200000.f / Math::inner(r, r) + 
        object.thrust + ( Math::length(object.velocity) ?
            Math::normalized(object.velocity) * planet.getAirDensity(Math::length(r) - planet.r) / 2.f * Math::inner(object.velocity, object.velocity) :
            Math::Vec3f{ 0.f, 0.f, 0.f }
        ) * 0.25f
    ) * dt;
    
    // Collision checking
    if (Math::length(object.position - planet.position) < planet.r + object.r)
    {
        object.position = Math::normalized(object.position - planet.position) * (planet.r + object.r) + planet.position;
        const auto normal = Math::normalized(object.position);
        object.velocity = (object.velocity - normal * 2.f * Math::inner(object.velocity, normal)) * 0.25f;
    }

    object.position += object.velocity * dt;

    offset = object.position;
}

void MainContext::draw_ui()
{
    const auto altitude = Math::length(object.position - planet.position) - planet.r;

    ImGui::Begin("Window");
    ImGui::Text("Distance: %f", distance);
    ImGui::Text("Vertices: %u, %lu kb", sphere->vertexCount(), sphere->vertexCount() * sizeof(Orbit::Vertex) / 1024);
    ImGui::SeparatorText("Data");
    ImGui::Text("Altitude: %f", altitude);
    ImGui::Text("Air density: %f", planet.getAirDensity( altitude ));
    ImGui::Text("Velocity: %f, %f, %f", Math::x(object.velocity), Math::y(object.velocity), Math::z(object.velocity));
    ImGui::Text("Thrust: %f, %f, %f", Math::x(object.thrust), Math::y(object.thrust), Math::z(object.thrust));
    ImGui::End();
}

void MainContext::draw_bg()
{   
    Math::z(rotation) = Math::Angle::radians(-acosf(Math::inner(Math::normalized(object.position), Math::Vec3f{ 0.f, 1.f, 0.f })));

    // At 25 most should be gone

    const auto proj = Math::perspective(_aspect, Math::Angle::degrees(50), { 0.01f, 10000.f });
    const auto view = Math::translation<float>(offset * -1.f) * Math::rotation<float>(rotation) * Math::translation<float>({ 0.f, 0.f, -distance });

    graphics_data->program.use();
    graphics_data->program.set("projection", proj);
    graphics_data->program.set("view", view);
    graphics_data->program.set("model", Math::scale<float>({ planet.r, planet.r, planet.r }) * Math::translation(planet.position));
    
    // Here is where we draw
    Graphics::draw(*sphere, graphics_data->program);

    graphics_data->program.set("model", Math::scale<float>({ object.r, object.r, object.r }) * Math::translation(object.position));
    Graphics::draw(*sphere, graphics_data->program);
}