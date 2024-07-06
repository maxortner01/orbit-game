#include <Graphics/Shader.hpp>

#include <GL/glew.h>

#include <sstream>
#include <fstream>

namespace Graphics
{
    
GLenum getType(Shader::Type type)
{
    switch (type)
    {
    case Shader::Vertex:   return GL_VERTEX_SHADER;
    case Shader::Fragment: return GL_FRAGMENT_SHADER;
    case Shader::Compute:  return GL_COMPUTE_SHADER;
    default: return 0;
    }
}

Shader::Shader(const std::filesystem::path& location, Type type)
{
    const auto contents = [location]()
    {
        std::ifstream file(location);
        ASSERT(file, "Invalid shader file");
        return (std::stringstream() << file.rdbuf()).str();
    }();

    const auto* c_str = contents.c_str();
    _handle = glCreateShader(getType(type));
    glShaderSource(_handle, 1, &c_str, nullptr);
    glCompileShader(_handle);

    int success;
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);
    ASSERT(success, [this]()
    {
        char infoLog[512];
        glGetShaderInfoLog(_handle, 512, nullptr, infoLog);
        return std::string(infoLog);
    }());
}

Shader::~Shader()
{
    glDeleteShader(_handle);
}

Program::Program(Program&& p) :
    _handle(p._handle)
{
    p._handle = 0;
}

Program::~Program()
{
    glDeleteProgram(_handle);
}

void Program::use() const
{
    glUseProgram(_handle);
}

void Program::set(const std::string& name, float v)
{
    glUniform1f(getLocation(name), v);
}

void Program::set(const std::string& name, double v)
{
    glUniform1d(getLocation(name), v);
}

void Program::set(const std::string& name, const Math::Vec2f& v)
{
    glUniform2f(getLocation(name), Math::x(v), Math::y(v));
}

void Program::set(const std::string& name, const Math::Vec3f& v)
{
    glUniform3f(getLocation(name), Math::x(v), Math::y(v), Math::z(v));
}


void Program::set(const std::string& name, const Math::Mat4<float>& m)
{
    glUniformMatrix4fv(getLocation(name), 1, false, &m.m[0][0]);
}

void Program::dispatch(const Math::Vec3u& workgroup) const
{
    use();
    glDispatchCompute(Math::x(workgroup), Math::y(workgroup), Math::z(workgroup));
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

int32_t Program::getLocation(const std::string& name)
{
    if (!uniforms.count(name))
    {
        const auto location = glGetUniformLocation(_handle, name.c_str());
        ASSERT(location > -1, "Uniform '" << name << "' not found.");
        uniforms[name] = location;
    }
    return uniforms[name];
}

ProgramBuilder& ProgramBuilder::addShader(const std::filesystem::path& file, Shader::Type type)
{
    ASSERT(!_shaders.count(type), "Shader already present!");

    if (type != Shader::Compute)
        ASSERT(!_shaders.count(Shader::Compute), "Compute shader must not be present");

    _shaders[type] = std::make_shared<Shader>(file, type);

    return *this;
}

Program ProgramBuilder::build() const
{
    auto program_id = glCreateProgram();

    for (const auto& p : _shaders)
        glAttachShader(program_id, p.second->getHandle());

    glLinkProgram(program_id);

    int success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    ASSERT(success, [program_id]()
    {
        char infoLog[512];
        glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
        return std::string(infoLog);
    }());

    Program p;
    p._handle = program_id;
    return p;
}

}