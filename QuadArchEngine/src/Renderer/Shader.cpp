#include "pch.h"
#include "Shader.h"

void QuadArch::Shader::LoadAndCompileShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	std::stringstream ss;

	std::ifstream vs(vertexShader);
	ss << vs.rdbuf();

	if (!this->CompileShader(ss.str(), GL_VERTEX_SHADER))
		std::cerr << "VERTEX SHADER: compilation failed!!" << std::endl;
	ss.clear();
	vs.close();

	std::ifstream fs(fragmentShader);
	ss << fs.rdbuf();

	if (!this->CompileShader(ss.str(), GL_FRAGMENT_SHADER))
		std::cerr << "FRAGMENT SHADER: compilation failed!!" << std::endl;
	ss.clear();
	fs.close();
}

void QuadArch::Shader::LoadAndCompileShader(const std::string& shaderText, unsigned int type)
{
	if (!this->CompileShader(shaderText, type))
		std::cerr << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") 
		         << ": compilation failed!!" << std::endl;
}

unsigned int QuadArch::ShaderProgram::GetUniformLocation(const char* name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	unsigned int location = glGetUniformLocation(m_ProgramID, name);
	if (location == -1)
	{
		std::cout << ("Uniform " + std::string(name) + " doesn't exists!!").c_str() << std::endl;
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
		break;
	case GL_FRAGMENT_SHADER:
		m_FShaderID = glCreateShader(type);
		glShaderSource(m_FShaderID, 1, &c_shader, NULL);
		glCompileShader(m_FShaderID);

		return this->ErrorLog(m_FShaderID);
		break;
	default:
		return false;
		break;
	}
	return true;
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
		
		std::cout << message << std::endl;
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
	glAttachShader(m_ProgramID, shader->GetVShaderID());
	glAttachShader(m_ProgramID, shader->GetFShaderID());

	this->m_Shaders.push_back(std::move(shader));
}

void QuadArch::ShaderProgram::Link() const
{
	glLinkProgram(m_ProgramID);

	GLint program_linked;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &program_linked);
	if (program_linked != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(m_ProgramID, 1024, &log_length, message);

		std::cout << message << std::endl;
	}
}

void QuadArch::ShaderProgram::Unbind()
{
	for (std::unique_ptr<Shader>& shader : m_Shaders)
	{
		glDeleteShader(shader->GetVShaderID());
		glDeleteShader(shader->GetFShaderID());
	}

	glDeleteProgram(m_ProgramID);
	m_Unbind = true;
}

void QuadArch::ShaderProgram::SetUniform1f(const char* name, float v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void QuadArch::ShaderProgram::SetUniform4f(const char* name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void QuadArch::ShaderProgram::SetUniformMat4(const char* name, glm::mat4 mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}