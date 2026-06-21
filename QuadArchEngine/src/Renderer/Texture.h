#pragma once
#include "QuadArch/Core.h"

#include <string>

namespace QuadArch
{
	struct TextureData
	{
		int width;
		int height;
		int nrChannels;

		unsigned char* data;
	};

	class QuadAPI Texture
	{
	public:
		Texture(const std::string& file_path);
		Texture(unsigned int rendererID);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void UnBind() const;
		unsigned int GetID() const { return m_TextureID; }

		static std::shared_ptr<Texture> LoadCubemap(const std::vector<std::string>& faces);
		static std::shared_ptr<Texture> LoadCrossCubemap(const std::string& file_path);
	private:
		TextureData m_data;
		unsigned int m_TextureID;
	};
}
