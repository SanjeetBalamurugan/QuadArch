#include "pch.h"
#include "Material.h"

std::unordered_map<QuadArch::MaterialType, std::unique_ptr<QuadArch::ShaderProgram>> QuadArch::Material::s_ShaderPipelines;

QuadArch::ShaderProgram* QuadArch::Material::GetShaderPipeline(MaterialType type)
{
    auto it = s_ShaderPipelines.find(type);
    if (it != s_ShaderPipelines.end())
        return it->second.get();
    return nullptr;
}

void QuadArch::Material::Bind() const
{
    ShaderProgram* program = GetShaderPipeline(m_Type);
    if (program)
    {
        program->UseShader();
    }

    if (m_texture)
    {
        glActiveTexture(GL_TEXTURE0);
        m_texture->Bind();
    }
}


void QuadArch::LitMaterial::Bind() const
{
    Material::Bind();

    ShaderProgram* program = Material::GetShaderPipeline(MaterialType::Lit);
    if (program)
    {
        program->SetUniform4f("u_Albedo", albedoColor.r, albedoColor.g, albedoColor.b, albedoColor.a);
        program->SetUniform1f("u_Roughness", roughness);
    }
}

void QuadArch::LitMaterial::CreateShader()
{
    const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord; 
layout (location = 3) in mat4 aModelMatrix;

uniform mat4 u_ViewProjection;

out vec2 TexCoord;
out vec4 VertexColor;

void main()
{
    gl_Position = u_ViewProjection * aModelMatrix * vec4(aPos, 1.0);
    
    TexCoord = aTexCoord;
    VertexColor = aColor;
}
)glsl";

    const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertexColor; 

uniform vec4 u_Albedo;
uniform float u_Roughness;
uniform sampler2D ourTexture;

void main()
{
    vec4 tex = texture(ourTexture, TexCoord);
    vec4 baseColor = (tex.a > 0.0 ? tex : vec4(1.0)) * u_Albedo;
    FragColor = baseColor * (1.0 - (u_Roughness * 0.5));
}
)glsl";

    auto shaderInstance = std::make_unique<Shader>();
    shaderInstance->LoadAndCompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    shaderInstance->LoadAndCompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    if (s_ShaderPipelines.find(MaterialType::Lit) == s_ShaderPipelines.end())
    {
        s_ShaderPipelines[MaterialType::Lit] = std::make_unique<ShaderProgram>();
    }

    auto& program = s_ShaderPipelines[MaterialType::Lit];
    program->Push(std::move(shaderInstance));
    program->Link();
}

QuadArch::SkyBoxMaterial::~SkyBoxMaterial()
{
    if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
}

void QuadArch::SkyBoxMaterial::DrawSkybox(const Camera3D& camera)
{
    if (m_VAO == 0)
    {
        InitSkyboxBuffers();
    }

    glDepthFunc(GL_LEQUAL);
    Bind();

    ShaderProgram* program = GetShaderPipeline(MaterialType::Skybox);
    if (program)
    {
        program->SetUniformMat4("u_View", camera.GetViewMatrix());
        program->SetUniformMat4("u_Projection", camera.GetProjectionMatrix());
    }

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void QuadArch::SkyBoxMaterial::Bind() const
{
    ShaderProgram* program = GetShaderPipeline(m_Type);
    if (program)
    {
        program->UseShader();
    }

    if (GetTexture())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, GetTexture()->GetID());
    }
}

void QuadArch::SkyBoxMaterial::CreateShader()
{
    if (s_ShaderPipelines.find(MaterialType::Skybox) != s_ShaderPipelines.end())
    {
        return;
    }

    const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 TexCoords;

void main()
{
    TexCoords = aPos;
    mat4 staticView = mat4(mat3(u_View)); 
    vec4 pos = u_Projection * staticView * vec4(aPos, 1.0);
    gl_Position = pos.xyww; 
}
)glsl";

    const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}
)glsl";

    auto shaderInstance = std::make_unique<Shader>();
    shaderInstance->LoadAndCompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    shaderInstance->LoadAndCompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    s_ShaderPipelines[MaterialType::Skybox] = std::make_unique<ShaderProgram>();
    auto& program = s_ShaderPipelines[MaterialType::Skybox];
    program->Push(std::move(shaderInstance));
    program->Link();
}

void QuadArch::SkyBoxMaterial::InitSkyboxBuffers()
{
    static const float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}
