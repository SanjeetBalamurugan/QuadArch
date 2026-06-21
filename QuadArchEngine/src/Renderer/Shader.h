#pragma once
#include "QuadArch/Core.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace QuadArch
{
	class QuadAPI Shader
	{
	public:
		void LoadAndCompileShader(const std::string& vertexShader, const std::string& fragmentShader);
		void LoadAndCompileShader(const std::string& shaderText, unsigned int type);

		unsigned int GetVShaderID() const { return m_VShaderID; }
		unsigned int GetFShaderID() const { return m_FShaderID; }
	private:

		unsigned int m_VShaderID = 0;
		unsigned int m_FShaderID = 0;

		bool CompileShader(const std::string& shader, unsigned int type);
		bool ErrorLog(unsigned int shaderID);
	};

	class QuadAPI ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) noexcept = default;
		ShaderProgram& operator=(ShaderProgram&&) noexcept = default;

		void Push(std::unique_ptr<Shader> shader);
		void Link();
		void UseShader() const;
		void Unbind();

		int GetUniformLocation(const std::string& name);

		void SetUniform1f(const std::string& name, float v0);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4(const std::string& name, const glm::mat4& mat);
	private:
		unsigned int m_ProgramID;
		bool m_Unbind = false;

		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::vector<std::unique_ptr<Shader>> m_Shaders;
	};
}