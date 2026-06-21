#include "pch.h"
#include "Shader.h"

void QuadArch::Shader::LoadAndCompileShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    std::stringstream ss;

    std::ifstream vs(vertexShader);
    if (vs.is_open()) {
        ss << vs.rdbuf();
        if (!this->CompileShader(ss.str(), GL_VERTEX_SHADER))
            std::cerr << "VERTEX SHADER: compilation failed!!" << std::endl;
        vs.close();
    }
    else {
        std::cerr << "Failed to open vertex shader file: " << vertexShader << std::endl;
    }

    ss.str("");
    ss.clear();

    std::ifstream fs(fragmentShader);
    if (fs.is_open()) {
        ss << fs.rdbuf();
        if (!this->CompileShader(ss.str(), GL_FRAGMENT_SHADER))
            std::cerr << "FRAGMENT SHADER: compilation failed!!" << std::endl;
        fs.close();
    }
    else {
        std::cerr << "Failed to open fragment shader file: " << fragmentShader << std::endl;
    }
}

void QuadArch::Shader::LoadAndCompileShader(const std::string& shaderText, unsigned int type)
{
    if (!this->CompileShader(shaderText, type))
        std::cerr << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
        << ": compilation failed!!" << std::endl;
}

int QuadArch::ShaderProgram::GetUniformLocation(const std::string& name)
{
    auto it = m_UniformLocationCache.find(name);
    if (it != m_UniformLocationCache.end())
        return it->second;

    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    if (location == -1)
    {
        std::cerr << "Uniform '" << name << "' doesn't exist (or was optimized out)!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}

bool QuadArch::Shader::CompileShader(const std::string& shader, unsigned int type)
{
    const char* c_shader = shader.c_str();
    switch (type)
    {
    case GL_VERTEX_SHADER:
        m_VShaderID = glCreateShader(type);
        glShaderSource(m_VShaderID, 1, &c_shader, NULL);
        glCompileShader(m_VShaderID);
        return this->ErrorLog(m_VShaderID);

    case GL_FRAGMENT_SHADER:
        m_FShaderID = glCreateShader(type);
        glShaderSource(m_FShaderID, 1, &c_shader, NULL);
        glCompileShader(m_FShaderID);
        return this->ErrorLog(m_FShaderID);

    default:
        return false;
    }
}

bool QuadArch::Shader::ErrorLog(unsigned int shaderID)
{
    GLint shader_compiled;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shader_compiled);
    if (shader_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderID, 1024, &log_length, message);

        std::cerr << "Shader Compile Error: " << message << std::endl;
        return false;
    }
    return true;
}

QuadArch::ShaderProgram::ShaderProgram()
{
    m_ProgramID = glCreateProgram();
}

QuadArch::ShaderProgram::~ShaderProgram()
{
    if (!m_Unbind)
        this->Unbind();
}

void QuadArch::ShaderProgram::Push(std::unique_ptr<Shader> shader)
{
    for (const auto& oldShader : m_Shaders)
    {
        if (oldShader->GetVShaderID() != 0) glDetachShader(m_ProgramID, oldShader->GetVShaderID());
        if (oldShader->GetFShaderID() != 0) glDetachShader(m_ProgramID, oldShader->GetFShaderID());
    }
    m_Shaders.clear();

    if (shader->GetVShaderID() != 0)
        glAttachShader(m_ProgramID, shader->GetVShaderID());

    if (shader->GetFShaderID() != 0)
        glAttachShader(m_ProgramID, shader->GetFShaderID());

    this->m_Shaders.push_back(std::move(shader));
}

void QuadArch::ShaderProgram::Link()
{
    glLinkProgram(m_ProgramID);

    GLint program_linked;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(m_ProgramID, 1024, &log_length, message);
        std::cerr << "Program Link Error: " << message << std::endl;
        return;
    }
    m_UniformLocationCache.clear();
}


void QuadArch::ShaderProgram::UseShader() const
{
    glUseProgram(m_ProgramID);
}

void QuadArch::ShaderProgram::Unbind()
{
    for (const auto& shader : m_Shaders)
    {
        if (shader->GetVShaderID() != 0) {
            glDetachShader(m_ProgramID, shader->GetVShaderID());
            glDeleteShader(shader->GetVShaderID());
        }
        if (shader->GetFShaderID() != 0) {
            glDetachShader(m_ProgramID, shader->GetFShaderID());
            glDeleteShader(shader->GetFShaderID());
        }
    }
    m_Shaders.clear();

    if (m_ProgramID != 0) {
        glDeleteProgram(m_ProgramID);
        m_ProgramID = 0;
    }
    m_Unbind = true;
}

void QuadArch::ShaderProgram::SetUniform1f(const std::string& name, float v0)
{
    glUniform1f(GetUniformLocation(name), v0);
}

void QuadArch::ShaderProgram::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void QuadArch::ShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}