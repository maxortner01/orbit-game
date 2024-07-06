#include "MainContext.hpp"

#include "Graphics/Draw.hpp"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>
#include <GL/glew.h>
#include <Math.hpp>

#ifndef FILES_DIR
#define FILES_DIR "."
#endif

#ifndef RES_DIR
#define RES_DIR "."
#endif

MainContext::MainContext() :
    selected{0},
    distance{1}
{
    ASSERT(!SDL_Init(SDL_INIT_VIDEO), "Error initializing SDL");

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    window = SDL_CreateWindow("Hello", 1920U, 1080U, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext( window );

    ASSERT(glewInit() == GLEW_OK, "Error initializing OpenGL");

    _aspect = 1920.f / 1080.f;

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

    current_dir = FILES_DIR;
    load_files();

    _open = true;

    graphics_data.emplace(Graphics::ProgramBuilder()
        .addShader(RES_DIR "/shaders/vertex.glsl",   Graphics::Shader::Vertex  )
        .addShader(RES_DIR "/shaders/fragment.glsl", Graphics::Shader::Fragment)
        .build());

    build_coord_mesh();
}

void MainContext::build_coord_mesh()
{
    std::vector<Vertex> coords;
    coords.reserve(6);

    const float extent = 500.f;
    const float line_extent = 1000.f;

    coords.push_back(Vertex{
        .position = { -extent, 0.f, 0.f },
        .color = { 1.f, 0.f, 0.f, 0.4f }
    });
    coords.push_back(Vertex{
        .position = { extent, 0.f, 0.f },
        .color = { 1.f, 0.f, 0.f, 0.4f }
    });
    
    coords.push_back(Vertex{
        .position = { 0.f, -extent, 0.f },
        .color = { 0.f, 1.f, 0.f, 0.4f }
    });
    coords.push_back(Vertex{
        .position = { 0.f, extent, 0.f },
        .color = { 0.f, 1.f, 0.f, 0.4f }
    });
    
    coords.push_back(Vertex{
        .position = { 0.f, 0.f, -extent },
        .color = { 0.f, 0.f, 1.f, 0.4f }
    });
    coords.push_back(Vertex{
        .position = { 0.f, 0.f, extent },
        .color = { 0.f, 0.f, 1.f, 0.4f }
    });

    for (int i = -line_extent; i < line_extent; i++)
    {
        coords.push_back(Vertex{
            .position = { i * extent / line_extent, 0.f, -extent },
            .color = { 0.1f, 0.1f, 0.1f, 0.2f }
        });
        coords.push_back(Vertex{
            .position = { i * extent / line_extent, 0.f, extent },
            .color = { 0.1f, 0.1f, 0.1f, 0.2f }
        });

        
        coords.push_back(Vertex{
            .position = { -extent, 0.f, i * extent / line_extent },
            .color = { 0.1f, 0.1f, 0.1f, 0.2f }
        });
        coords.push_back(Vertex{
            .position = { extent, 0.f, i * extent / line_extent },
            .color = { 0.1f, 0.1f, 0.1f, 0.2f }
        });
    }

    graphics_data->vao.getVertexBuffer()->setData(coords);
    graphics_data->vao.addVertexAttribute(0, 3, sizeof(Vertex), 0);
    graphics_data->vao.addVertexAttribute(1, 4, sizeof(Vertex), sizeof(Math::Vec3f));

    {
        std::vector<Vertex> vertices(10, {});
        graphics_data->indicator_lines.getVertexBuffer()->setData(vertices);
        graphics_data->indicator_lines.addVertexAttribute(0, 3, sizeof(Vertex), 0);
        graphics_data->indicator_lines.addVertexAttribute(1, 4, sizeof(Vertex), sizeof(Math::Vec3f));
    }
}

void MainContext::load_files()
{
    for (const auto & entry : std::filesystem::directory_iterator(current_dir))
        if (entry.path().extension() == ".h5")
            avaliable_files.push_back(entry.path());
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
    glClearColor(0.2f, 0.3f, 0.4f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainContext::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    glDisable(GL_DEPTH_TEST);
    
    draw_ui();

    clear();
    ImGui::Render();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    draw_bg();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void MainContext::pollEvents()
{
    static bool left_down = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (ImGui::GetIO().WantCaptureMouse) { left_down = false; continue; }

        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: _open = false; break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                left_down = true;
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT)
                left_down = false;
            break;

        case SDL_EVENT_MOUSE_WHEEL:

            distance -= distance * event.wheel.y * 0.2f;

            break;

        case SDL_EVENT_MOUSE_MOTION:

            if (left_down)
            {
                Math::y(rotation) -= Math::Angle::degrees(event.motion.xrel * 0.2f);
                Math::x(rotation) -= Math::Angle::degrees(event.motion.yrel * 0.2f);
            }

            break;
        default: break;
        }
    }
}

void MainContext::draw_ui()
{
    if (!flight_data)
    {
        ImGui::Begin("Load Flight Data");
        if (!avaliable_files.size())
            ImGui::Text("No available files...");
        else
        {
            std::vector<std::string> strs(avaliable_files.size());
            for (uint32_t i = 0; i < avaliable_files.size(); i++) strs[i] = avaliable_files[i].filename().string();
            std::vector<const char*> fs(avaliable_files.size());
            for (uint32_t i = 0; i < avaliable_files.size(); i++) fs[i] = strs[i].c_str();

            ImGui::ListBox("Files", &selected, fs.data(), static_cast<int>(fs.size()));
            if (ImGui::Button("Open"))
                flight_data.emplace(avaliable_files[selected]);
        }
        ImGui::End();
    }
    // Draw flight data UI
    else
    {
        ImGui::Begin("Flight Data");

        ImGui::SeparatorText("Time Control");

        const auto time = ( flight_data->currentStep() ? flight_data->getFrames().back().time : 0.0 );
        ImGui::Text((std::stringstream() << "Time: " << time << " seconds").str().c_str()); 

        ImGui::Text((std::stringstream() << "Frame " << flight_data->currentStep() << "/" << flight_data->timeSteps()).str().c_str());

        if (flight_data->currentStep())
        {
            const auto& current_frame = flight_data->getFrames()[flight_data->currentStep() - 1];
            ImGui::Text((std::stringstream() << "Current pos: " << Math::x(current_frame.center_of_mass) << ", " << Math::y(current_frame.center_of_mass) << ", " << Math::z(current_frame.center_of_mass)).str().c_str());
        }

        ImGui::SeparatorText("Camera Control");
        ImGui::Checkbox("Follow center of mass", &follow);

        ImGui::SeparatorText("Engines");
        const auto frames = flight_data->getFrames();
        const std::vector<std::string> engines = { "tr1", "tl1", "tr2", "tl2" };
        
        ImPlotSubplotFlags flags = ImPlotSubplotFlags_LinkRows | ImPlotSubplotFlags_LinkCols;

        ImPlot::BeginSubplots("Thrusts", 4, 1, ImVec2(-1,600), flags);
        for (uint32_t i = 0; i < 4; i++)
        {
            if (!frames.size()) break;
            std::vector<double> ts(frames.size());
            std::vector<double> vals(frames.size());
            for (uint32_t j = 0; j < frames.size(); j++)
            {
                ts[j] = frames[j].time;
                vals[j] = frames[j].engines.at(engines[i]).thrust;
            }

            const auto val = ts.back();
            ImPlot::BeginPlot(engines[i].c_str());
            ImPlot::SetupAxesLimits(ts.back() - 4.0, ts.back(), 0, 4, ImGuiCond_Always);
            ImPlot::PlotLine(engines[i].c_str(), &ts.front(), &vals.front(), vals.size());
            ImPlot::EndPlot();

        }
        ImPlot::EndSubplots();

        ImGui::End();
    }
}

void MainContext::draw_bg()
{
    static uint32_t index = 0;
    // Render stuff
    const auto proj = Math::perspective(_aspect, Math::Angle::degrees(50), { 0.01f, 1000.f });
    const auto view = Math::translation<float>(offset * -1.f) * Math::rotation<float>(rotation) * Math::translation<float>({ 0.f, 0.f, -distance });

    graphics_data->program.use();
    graphics_data->program.set("projection", proj);
    graphics_data->program.set("view", view);

    Graphics::draw(graphics_data->vao, graphics_data->program, Graphics::Primitive::Lines);
    
    if (flight_data)
        flight_data->setCurrentStep(index++ * 8);

    if (flight_data && flight_data->currentStep())
    {
        const auto& frame = flight_data->getFrames().back();
        if (follow)
            offset = frame.center_of_mass;
        else 
            offset = Math::Vec3f();

        Graphics::draw(flight_data->getCOM(), graphics_data->program, Graphics::Primitive::Lines, flight_data->currentStep() * 2);

        // Generate and draw the indicator lines
        {
            std::vector<Vertex> vertices;
            vertices.reserve(10);
            vertices.push_back(Vertex{
                .position = { (float)Math::x(frame.center_of_mass), 0.f, (float)Math::z(frame.center_of_mass) },
                .color    = { 1.f, 0.f, 0.f, 0.8f }
            });
            vertices.push_back(Vertex{
                .position = (Math::Vec3f)frame.center_of_mass,
                .color    = { 1.f, 0.f, 0.f, 0.8f }
            });

            for (const auto& [ name, vao ] : flight_data->engineVAOS())
            {
                vertices.push_back(Vertex{
                    .position = { (float)Math::x(frame.engines.at(name).position + frame.center_of_mass), 0.f, (float)Math::z(frame.engines.at(name).position + frame.center_of_mass) },
                    .color    = { 1.f, 0.f, 0.f, 0.6f }
                });
                vertices.push_back(Vertex{
                    .position = (Math::Vec3f)(frame.engines.at(name).position + frame.center_of_mass),
                    .color    = { 1.f, 0.f, 0.f, 0.6f }
                });
                Graphics::draw(vao, graphics_data->program, Graphics::Primitive::Lines, flight_data->currentStep() * 2);
            }

            graphics_data->indicator_lines.getVertexBuffer()->setData(vertices);
        }
        Graphics::draw(graphics_data->indicator_lines, graphics_data->program, Graphics::Primitive::Lines);

        // Draw the desired force vector
        {
            std::vector<Vertex> vertices(2);

            vertices[0] = Vertex {
                .position = (Math::Vec3f)frame.center_of_mass,
                .color = { 0.f, 0.f, 1.f, 1.f }
            };
            vertices[1] = Vertex {
                .position = (Math::Vec3f)(frame.center_of_mass + frame.desired_force),
                .color = { 0.f, 0.f, 1.f, 1.f }
            };

            flight_data->getDesiredForce().getVertexBuffer()->setData(vertices);
        }
        Graphics::draw(flight_data->getDesiredForce(), graphics_data->program, Graphics::Primitive::Lines);
        
        // [x] !! also a button to center camera on plane position of drone
        // we want to output the parameter info (thrusts for example, and possibly desired force, etc) to the hdf5 file and read that in and display it using implot
        // Need to be able to draw arrows for thrust forces, desired force, and net force, etc
        // Toggles for all these options

        // Next we want to draw a sphere where the current position is
        // Lines for the engine positions
        // Spheres for the current engine positions
        // lines from the current pos of engines and the COM down to the plane
    }
}