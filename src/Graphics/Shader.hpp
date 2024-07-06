#pragma once

#include <Def.hpp>
#include <Math.hpp>
#include <unordered_map>

namespace Graphics
{
    struct Shader
    {
        enum Type
        {
            Vertex, Fragment, Compute
        };
        
        Shader(const std::filesystem::path& location, Type type);

        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;

        ~Shader();

        auto getHandle() const { return _handle; }

    private:
        uint32_t _handle;
    };

    struct ProgramBuilder;

    struct Program
    {
        friend struct ProgramBuilder;

        Program(const Program&) = delete;
        Program(Program&&);

        ~Program();

        void use() const;

        void set(const std::string& name, float v);
        void set(const std::string& name, double v);
        void set(const std::string& name, const Math::Vec2f& v);
        void set(const std::string& name, const Math::Vec3f& v);
        void set(const std::string& name, const Math::Mat4<float>& v);

        void dispatch(const Math::Vec3u& workgroup) const;

    private:
        int32_t getLocation(const std::string& name);

        Program() = default;

        uint32_t _handle;

        std::unordered_map<std::string, int32_t> uniforms;
    };

    struct ProgramBuilder
    {
        ProgramBuilder& addShader(const std::filesystem::path& file, Shader::Type type);
        
        Program build() const;

    private:
        std::unordered_map<Shader::Type, std::shared_ptr<Shader>> _shaders;
    };
}