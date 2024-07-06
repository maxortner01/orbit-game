#pragma once

#include <filesystem>
#include <string>
#include <span>
#include <unordered_map>

#include "Graphics/Buffer.hpp"
#include <Math.hpp>

struct FlightData
{
    struct Frame
    {
        struct Engine
        {
            Math::Vec3d position;
            double thrust;
        };

        std::unordered_map<std::string, Engine> engines;
        Math::Vec3d center_of_mass, desired_force;
        double time;
    };

    FlightData(const std::filesystem::path& file);

    uint32_t timeSteps() const { return frames.size(); }
    uint32_t currentStep() const { return current_time_step; }
    void setCurrentStep(uint32_t c) { current_time_step = ( c % frames.size() ); }

    std::span<const Frame> getFrames() const
    {
        return { frames.begin(), current_time_step };
    }

    const auto& getCOM() const { return com; }
    const auto& getEngineVAO(const std::string& name) const { return engine_vao.at(name); }
    const auto& engineVAOS() const { return engine_vao; }
    auto& getDesiredForce() { return desired_force; }

private:
    Graphics::VAO com;
    Graphics::VAO desired_force;
    std::unordered_map<std::string, Graphics::VAO> engine_vao;

    uint32_t current_time_step;
    std::vector<Frame> frames;
};