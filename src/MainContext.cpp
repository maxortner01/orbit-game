#include "MainContext.hpp"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>
#include <GL/glew.h>

MainContext::MainContext()
{
    ASSERT(!SDL_Init(SDL_INIT_VIDEO), "Error initializing SDL");

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    window = SDL_CreateWindow("Hello", 1280U, 720U, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext( window );

    ASSERT(glewInit() == GLEW_OK, "Error initializing OpenGL");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.Fonts->AddFontFromFileTTF(SOURCE_DIR "/res/fonts/Karla-Regular.ttf", 16);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();

    _open = true;
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
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainContext::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    draw_ui();

    ImGui::Render();
    clear();

    draw_bg();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void MainContext::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: _open = false; break;
        default: break;
        }
    }
}

void MainContext::draw_ui()
{
    ImGui::Begin("Hello");
    ImGui::Text("Test");
    ImGui::End();
}

void MainContext::draw_bg()
{
    
}