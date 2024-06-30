#pragma once

#include <string>

#include "MeshBuilder.hpp"

struct FlightData
{
    FlightData(const std::string& file);

private:
    Mesh lines;
};