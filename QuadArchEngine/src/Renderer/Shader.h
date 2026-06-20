#pragma once
#include "QuadArch/Core.h"

#include <string>
#include <memory>
#include <vector>

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

		void Push(std::unique_ptr<Shader> shader);
		void Link() const;
		void Unbind();

		unsigned int GetUniformLocation(const char* name);

		void SetUniform1f(const char* name, float v0);
		void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
		void SetUniformMat4(const char* name, glm::mat4 mat);
	private:
		unsigned int m_ProgramID;
		bool m_Unbind = false;

		std::unordered_map<const char*, int> m_UniformLocationCache;
		std::vector<std::unique_ptr<Shader>> m_Shaders;
	};
}