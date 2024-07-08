#include "Primitives.hpp"

namespace Orbit
{
    Graphics::VAO Primitive::createSphere(float r, int32_t res)
    {
        // Construct a box with (res x res) sides
        std::vector<Vertex> vertices;
        //std::vector<uint32_t> indices;

        const auto f_res = static_cast<float>(res);
        const auto inv_f_res = 1.f / f_res;
        for (int i = -res; i < res; i++)
            for (int j = -res; j < res; j++)
            {
                const float x = (float)j / f_res;
                const float y = (float)i / f_res;
                // Back
                {
                    vertices.push_back(Vertex{
                        .position = { x, y, 1.f },
                        .normal = { 0.f, 0.f, 1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x + inv_f_res, y, 1.f },
                        .normal = { 0.f, 0.f, 1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x, y + inv_f_res, 1.f },
                        .normal = { 0.f, 0.f, 1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });

                    vertices.push_back(Vertex{
                        .position = { x, y + inv_f_res, 1.f },
                        .normal = { 0.f, 0.f, 1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x + inv_f_res, y, 1.f },
                        .normal = { 0.f, 0.f, 1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x + inv_f_res, y + inv_f_res, 1.f },
                        .normal = { 0.f, 0.f, 1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                }

                // Front
                {
                    vertices.push_back(Vertex{
                        .position = { x, y, -1.f },
                        .normal = { 0.f, 0.f, -1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x, y + inv_f_res, -1.f },
                        .normal = { 0.f, 0.f, -1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x + inv_f_res, y, -1.f },
                        .normal = { 0.f, 0.f, -1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });

                    vertices.push_back(Vertex{
                        .position = { x, y + inv_f_res, -1.f },
                        .normal = { 0.f, 0.f, -1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x + inv_f_res, y + inv_f_res, -1.f },
                        .normal = { 0.f, 0.f, -1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { x + inv_f_res, y, -1.f },
                        .normal = { 0.f, 0.f, -1.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                }
            
                // Left
                {
                    vertices.push_back(Vertex{
                        .position = { -1.f, x, y },
                        .normal = { -1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { -1.f, x, y + inv_f_res },
                        .normal = { -1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { -1.f, x + inv_f_res, y },
                        .normal = { -1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });

                    vertices.push_back(Vertex{
                        .position = { -1.f, x, y + inv_f_res },
                        .normal = { -1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { -1.f, x + inv_f_res, y + inv_f_res },
                        .normal = { -1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { -1.f, x + inv_f_res, y },
                        .normal = { -1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                }
            
                // Right
                {
                    vertices.push_back(Vertex{
                        .position = { 1.f, x, y },
                        .normal = { 1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { 1.f, x + inv_f_res, y },
                        .normal = { 1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { 1.f, x, y + inv_f_res },
                        .normal = { 1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });

                    vertices.push_back(Vertex{
                        .position = { 1.f, x, y + inv_f_res },
                        .normal = { 1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { 1.f, x + inv_f_res, y },
                        .normal = { 1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { 1.f, x + inv_f_res, y + inv_f_res },
                        .normal = { 1.f, 0.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                }
            
                // Top
                {
                    vertices.push_back(Vertex{
                        .position = { y, 1.f, x },
                        .normal = { 0.f, 1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y, 1.f, x + inv_f_res },
                        .normal = { 0.f, 1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y + inv_f_res, 1.f, x },
                        .normal = { 0.f, 1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });

                    vertices.push_back(Vertex{
                        .position = { y + inv_f_res, 1.f, x },
                        .normal = { 0.f, 1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y, 1.f, x + inv_f_res },
                        .normal = { 0.f, 1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y + inv_f_res, 1.f, x + inv_f_res },
                        .normal = { 0.f, 1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                }
            
                // Bottom
                {
                    vertices.push_back(Vertex{
                        .position = { y, -1.f, x },
                        .normal = { 0.f, -1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y + inv_f_res, -1.f, x },
                        .normal = { 0.f, -1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y, -1.f, x + inv_f_res },
                        .normal = { 0.f, -1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });

                    vertices.push_back(Vertex{
                        .position = { y + inv_f_res, -1.f, x },
                        .normal = { 0.f, -1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y + inv_f_res, -1.f, x + inv_f_res },
                        .normal = { 0.f, -1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                    vertices.push_back(Vertex{
                        .position = { y, -1.f, x + inv_f_res },
                        .normal = { 0.f, -1.f, 0.f },
                        .color = { 1.f, 1.f, 1.f, 1.f }
                    });
                }
            }

        // Push all vertices to distance $r$ from center
        
        for (auto& vertex : vertices)
        {
            vertex.position = Math::normalized(vertex.position) * r;
            vertex.normal = Math::normalized(vertex.position);
        }


        Graphics::VAO vao;
        vao.getVertexBuffer()->setData(vertices);
        vao.addVertexAttribute(0, 3, sizeof(Vertex), 0);
        vao.addVertexAttribute(1, 3, sizeof(Vertex), sizeof(Math::Vec3f));
        vao.addVertexAttribute(2, 4, sizeof(Vertex), 2 * sizeof(Math::Vec3f));
        return vao;
    }
}