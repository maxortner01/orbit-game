#include "FlightData.hpp"

#include <highfive/highfive.hpp>
#include <iostream>
#include <Math.hpp>

#include "MainContext.hpp"

HighFive::CompoundType create_compound_vec() {
    return {
        {"x", HighFive::AtomicType<double>{}}, 
        {"y", HighFive::AtomicType<double>{}},
        {"z", HighFive::AtomicType<double>{}}
    };
}

HIGHFIVE_REGISTER_TYPE(Math::Vec3<double>, create_compound_vec)

template<typename T>
std::vector<T> get_dataset(HighFive::File& file, const std::string& name)
{
    auto dataset = file.getDataSet(name);
    auto data = dataset.read<std::vector<T>>();
    dataset.read(data);
    return data;
}

FlightData::FlightData(const std::filesystem::path& filename) :
    current_time_step{0}
{
    using HighFive::File;

    std::cout << "Opening flight data file: " << filename.string() << "\n";
    std::cout << "Loading flight data...";

    // We open the file as read-only:
    File file(filename.string(), File::ReadOnly);    

    const auto swap_dataset = [](auto& data)
    {
        for (auto& p : data)
            std::swap(Math::y(p), Math::z(p));
    };

    const auto center_of_mass = [&file, &swap_dataset]()
    {
        auto dataset = file.getDataSet("/center_of_mass/positions");
        auto data = dataset.read<std::vector<Math::Vec3d>>();
        dataset.read(data);
        swap_dataset(data);
        return data;
    }();

    {
        std::vector<Vertex> vertices;

        frames.resize(center_of_mass.size());
        for (uint32_t i = 0; i < center_of_mass.size(); i++)
        {
            frames[i].center_of_mass = center_of_mass[i];

            if (i >= 1)
            {
                vertices.push_back(Vertex{ 
                    .position = (Math::Vec3f)frames[i - 1].center_of_mass,
                    .color = { 0.f, 0.f, 0.f, 1.f }
                });
                
                vertices.push_back(Vertex{ 
                    .position = (Math::Vec3f)frames[i].center_of_mass,
                    .color = { 0.f, 0.f, 0.f, 1.f }
                });
            }
        }
        com.getVertexBuffer()->setData(vertices);
        com.addVertexAttribute(0, 3, sizeof(Vertex), 0);
        com.addVertexAttribute(1, 4, sizeof(Vertex), sizeof(Math::Vec3f));
    }

    const std::vector<std::string> engines = { "tr1", "tl1", "tr2", "tl2" };
    
    for (const auto& engine : engines)
    {
        std::vector<Vertex> vertices;
        auto pos_data = get_dataset<Math::Vec3d>(file, "/" + engine + "/positions");
        swap_dataset(pos_data);
        const auto thrust_data = get_dataset<double>(file, "/" + engine + "/thrusts");

        for (uint32_t i = 0; i < pos_data.size(); i++)
        {
            const auto pos = center_of_mass[i] + pos_data[i];
            frames[i].engines[engine].position = pos_data[i];
            frames[i].engines[engine].thrust = thrust_data[i];

            if (i >= 1)
            {
                vertices.push_back(Vertex{ 
                    .position = (Math::Vec3f)(center_of_mass[i - 1] + pos_data[i - 1]),
                    .color = { 0.f, 0.f, 0.f, 1.f }
                });
                
                vertices.push_back(Vertex{ 
                    .position = (Math::Vec3f)(center_of_mass[i] + pos_data[i]),
                    .color = { 0.f, 0.f, 0.f, 1.f }
                });
            }
        }
        engine_vao[engine].getVertexBuffer()->setData(vertices);
        engine_vao[engine].addVertexAttribute(0, 3, sizeof(Vertex), 0);
        engine_vao[engine].addVertexAttribute(1, 4, sizeof(Vertex), sizeof(Math::Vec3f));
    }
    
    // Read time from file
    {
        const auto data = get_dataset<double>(file, "/time");
        for (uint32_t i = 0; i < data.size(); i++)
            frames[i].time = data[i];
    }
    
    // Read desired force from file
    {
        const auto data = get_dataset<Math::Vec3d>(file, "/desired_force");
        for (uint32_t i = 0; i < data.size(); i++)
            frames[i].desired_force = data[i];

        std::vector<Vertex> points(2);
        desired_force.getVertexBuffer()->setData(points);
        desired_force.addVertexAttribute(0, 3, sizeof(Vertex), 0);
        desired_force.addVertexAttribute(1, 4, sizeof(Vertex), sizeof(Math::Vec3f));
    }

    std::cout << "done\n";
}
