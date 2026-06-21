#pragma once
#include "QuadArch/Core.h"
#include "Shader.h"
#include "Texture.h"

namespace QuadArch
{
	enum class MaterialType {
		Lit,
		Unlit,
		Skybox
	};

	class QuadAPI Material
	{
	public:
		Material(unsigned int id, std::shared_ptr<Texture> texture, MaterialType type)
			: m_MaterialID(id), m_texture(std::move(texture)), m_Type(type) {}
		virtual ~Material() = default;

		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
		Material(Material&&) noexcept = default;
		Material& operator=(Material&&) noexcept = default;

		virtual void Bind() const;

		static ShaderProgram* GetShaderPipeline(MaterialType type);
		unsigned int GetMaterialID() const { return m_MaterialID; }
		MaterialType GetType() const { return m_Type; }

	protected:
		static std::unordered_map<MaterialType, std::unique_ptr<ShaderProgram>> s_ShaderPipelines;
		MaterialType m_Type;

	private:
		unsigned int m_MaterialID;
		std::shared_ptr<Texture> m_texture;
	};

	class QuadAPI LitMaterial : public Material {
	public:
		glm::vec4 albedoColor;
		float roughness;

		LitMaterial(unsigned int id, std::shared_ptr<Texture> texture, const glm::vec4& albedoColor, float rough = 0.9f)
			: Material(id, std::move(texture), MaterialType::Lit), albedoColor(albedoColor), roughness(rough) 
		{
			this->CreateShader();
		}

		void Bind() const override;
		static void CreateShader();
	};
}
