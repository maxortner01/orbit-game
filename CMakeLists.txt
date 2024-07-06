cmake_minimum_required(VERSION 3.8)

project(main CXX)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_PREFIX_PATH "C:/hdf5/cmake")
add_subdirectory(extern/glew-cmake)
add_subdirectory(extern/SDL)
add_subdirectory(extern/HighFive)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/Graphics/Buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/Graphics/Draw.cpp
    ${CMAKE_SOURCE_DIR}/src/Graphics/Framebuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/Graphics/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/Graphics/Texture.cpp

    ${CMAKE_SOURCE_DIR}/src/Math/Angle.cpp

    ${CMAKE_SOURCE_DIR}/src/MainContext.cpp
    ${CMAKE_SOURCE_DIR}/src/FlightData.cpp)

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/extern/imgui/imgui.h
    ${CMAKE_SOURCE_DIR}/extern/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/extern/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/extern/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/extern/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/extern/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/extern/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/extern/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/extern/imgui/backends/imgui_impl_sdl3.cpp)

set(IMPLOT_SOURCES
    ${CMAKE_SOURCE_DIR}/extern/implot/implot.h
    ${CMAKE_SOURCE_DIR}/extern/implot/implot.cpp
    ${CMAKE_SOURCE_DIR}/extern/implot/implot_internal.h
    ${CMAKE_SOURCE_DIR}/extern/implot/implot_items.cpp)

add_executable(main main.cpp ${SOURCES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES})
target_link_libraries(main PRIVATE libglew_static SDL3::SDL3 HighFive)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/extern/imgui ${CMAKE_SOURCE_DIR}/extern/implot ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(main PRIVATE -DFILES_DIR="${CMAKE_SOURCE_DIR}/data" -DRES_DIR="${CMAKE_SOURCE_DIR}/res")